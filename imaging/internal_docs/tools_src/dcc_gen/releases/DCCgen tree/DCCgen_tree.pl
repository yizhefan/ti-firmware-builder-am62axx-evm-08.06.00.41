#!/usr/bin/perl

use strict;
use File::Basename;
use File::Spec;
use Cwd;
use File::Copy;
use Getopt::Long;
use POSIX;

###########################################################
# VARS
###########################################################

$| = 1; #set autoflush mode on STDOUT

my %CONF = ();

my %DBG = (
'l1' => 'level_1,l1,1', 
'l2' => 'level_2,l2,2', 
'l3' => 'level_3,l3,3',
'v'  => 'v,verbose',
'print_history' => 'print_history',
'cmd_out'       => 'cmd_out'
);


my $USAGE = <<"USAGE_EN";

SYNOPSIS: 

OPTIONS:
  -d | --root-dir       [REQUIRED] Set path to directory, which shall be 
                        recursively proccessed for generating DCC 
                        stuff. 

  -p | --dir-pattern    [REQUIRED] Set pattern, which shall define, which directories
                        to proccess. Pattern is assumed to be regular 
                        expression

  -t | --dccgen         [REQUIRED] Set tool, which shall generates DCC stuff                      

       --dir-separator 

       --dcc_result_locations-file-name

       --dry-run        Execute DCC generator for every xml, but DOES NOT copy/install
                        any files. Just prints, where files shall be installed.

Ancillary Options:
       --debug        print some helpful debug information. Accepted values: 
                        level_1

  -h | --help         display this help.


dcc_result_location.txt format: 
<path to copy H file>
<path to copy C file>
<path to copy bin file>
--EOF--

i.e. it contains 3 lines. Every line defines relative path, 
where to install corresponding product file. 

The paths are relative to the folder @{[basename $0]} is called from 
(building folder)


Examples: 
=========

@{[basename $0]} --root-dir /path/to/SW_root  --dir-pattern dccxml_ --dccgen DCCgen.exe

USAGE_EN


###########################################################
# SUBROUTINES
###########################################################

sub print_hash($$%) {
  die "PrintHash: there is no input arguments!\n"
    unless @_;

  my $indent = shift;
  my $header_msg = shift;
  my $hashref = ref($_[0]) eq 'HASH' ? shift : {@_};

  $header_msg = "Elements of hash is" 
      unless (defined($header_msg) and $header_msg ne "");

  #map { $field_len =  } foreach (keys %$hashref);

  print "${header_msg}: \n";
  foreach (keys %$hashref) {
    if (ref $hashref->{$_} eq 'HASH') {
      print_hash("${indent}  ", ${indent} . uc($_), %{$hashref->{$_}});
    } else { 
      printf "${indent}%-20s  =>  %s\n", $_, $hashref->{$_};
    }
  }
}

sub set_defaults(@) {
  %CONF = ( 
    'root_dir'      => cwd,
    'dir_pattern'   => 'dccxml_',
    'dccgen'        => cwd . "DCCgen.exe",
    'dir_sep'       =>  do { $_ = $^O; /MSWin32/i && '\\'|| /linux/i && '/' || die 'Unsupported OS'; },
    
    # Every dccxml_xxx dir contains a file, which holds releatives path(s), 
    # where to copy output file(s)
    'dcc_result_location' => 'dcc_result_location.txt',
    'dcc_xml_pattern'     => '.xml',
    'dcc_xml_suffix'      => '.xml',
    'dcc_bin_suffix'      => '.bin',
    'debug'               => 'none',
    'list'                => 0,
    'dry_run'             => 0,
  );
  
}

sub parse_cmd(@) {
  GetOptions(
    'root-dir|d=s'      => \$CONF{'root_dir'}, 
    'dir-pattern|p=s'   => \$CONF{'dir_pattern'},
    'dccgen|t=s'        => \$CONF{'dccgen'},
    'dir-separator=s'   => \$CONF{'dir_sep'},
    'help|h'            => sub { print "$USAGE"; exit 0; },
    'debug=s'           => \$CONF{'debug'},
    'dcc_result_locations-file-name=s' => \$CONF{'dcc_result_location'},
    'list|l'            => \$CONF{'list'}, 
    'dry-run'           => \$CONF{'dry_run'},
    ) || die do { print "${USAGE}"; exit 1; };
} 

sub verify_config() {
  $CONF{'root_dir'} = File::Spec->rel2abs($CONF{'root_dir'});

  $CONF{'debug'} = join('|', split(',', $CONF{'debug'}));
  $CONF{'debug'} = qr/$CONF{'debug'}/;

  $CONF{'dccgen'} = File::Spec->rel2abs($CONF{'dccgen'});

  # FILL ME .....
}

# Remove and CONVERT extra slashesh according to detected OS
# e.g. for win32 we have \ as directory separator
# e.g. for linux we have / as directory separator
# and append slash at the end if 2-nd arg is true (or 1)
# NOTE: modify 1-st arg
sub slashes(\$;$) {
  my $dir = shift;
  my $flag = defined($_[0]) ? shift : 1;
  return if not defined $dir;

  $$dir =~ s/((\\)+|(\/)+)+/$CONF{'dir_sep'}/g;
  $$dir .= $CONF{'dir_sep'} if $flag && substr($$dir, -1, 1) ne $CONF{'dir_sep'};
  return $$dir;
}

sub wait_user
{ 
  my $dummy;
  my $default_message="Press [ENTER] to continue ...\n\n";

  printf +($#_ eq 0 ?  "@_" : $default_message);
  $dummy = <STDIN>;
}

sub get_dcc_fnames(\%$) {
  my ($r, $xml) = (@_);
  my $fd;

  local $_;
  local $/ = undef;
 
  open($fd, '<', $xml) or return 1; 
  $_ = <$fd>;
  close $fd;
  
  $$r{'h'}{'src'} = ( m|\<dcc_name.*?\>\s*(.*?)\s*\</dcc_name\>|ms ? "${1}" : undef );

  if ( defined($$r{'h'}{'src'}) ) {
    # Precaution if h file name has been broken down on several lines (on mistake)
    # This line will clean newline character(s)
    $$r{'h'}{'src'} =~ s/[\n\r]+//g; 
    $$r{'c'}{'src'} = "$$r{'h'}{'src'}.c";
    $$r{'h'}{'src'} = "$$r{'h'}{'src'}.h";
  }

  $$r{'cid'} = ( m|\<camera_module_id.*?\>\s*(.*?)\s*\</.*?camera_module_id>| 
                 ? "${1}" : undef);

  return 0;
}

sub get_install_paths(\%$) {
  my ($r, $locations_file) = (@_); 
  my @lines = ();
  my $fd;

  local $_;

  open($fd, '<', $locations_file) or return 1; 
  @lines = <$fd>;
  close $fd;

  for ('bin', 'c', 'h') {
    $$r{$_}{'dst'} = pop(@lines);
    $$r{$_}{'dst'} =~ s/[\r\n]+$//s;
    slashes($$r{$_}{'dst'}, 0);
  }

  return 0;
}

sub dccgen_tree(%) { 
  my %conf = (@_);

  my $dir = undef
  my $dirh = undef;
  my $prev = undef;
  my $cur = undef;
  my $crc_prev = 0;
  my $crc_cur = 0;
  my $idx = 0;
  my %dcc = ();
  my @dirs = (find(@conf{'root_dir', 'dir_pattern', 'dir_sep'}, 0, 0));
  my %history = ();
  my $xml_id = 0;
  my @files = ('bin', 'h', 'c');

  my $cleanup = sub {
    my $err = 0;
    local $_;
   
    FILE: foreach $_ (@_) {
      printf "DELETE: 'ROOT_DIR$conf{'dir_sep'}" . File::Spec->abs2rel($history{$idx}{$_}{'src'}, $conf{'root_dir'}) . "'\n" ;

      unless ($CONF{'dry_run'}) {
        unless (unlink $history{$idx}{$_}{'src'}) {
          printf STDERR
            "ERROR: failed to DELETE:\n\n  '$history{$idx}{$_}{'src'}'\n\n"
            . "  check that the file exists and is WRITABLE\n\n";
          $err++;
          next FILE;
        }
      }
    };

    return $err;
  };

  my $install = sub {
    my $err = 0;
    my $flag = $CONF{'dry_run'};
    local $_;

    FILE: foreach $_ (@_) {
      unless ($CONF{'dry_run'}) {
        $flag = move($history{$idx}{$_}{'src'}, $history{$idx}{$_}{'dst'});
        unless ($flag) {
          printf STDERR 
              "ERROR: failed to MOVE:\n\n  '$history{$idx}{$_}{'src'}'\n\n"
            . "  Check that:\n"
            . "  * file exists and is READABLE/WRITABLE\n"
            . "  * DST directory exists and is WRITABLE\n"
            . "  * file name set in XML is correct. Pay attention for whitespaces!!!\n\n";
          $err++;
          next FILE;
        }
      }
      if ($flag) {
        printf ("INSTALL: 'ROOT_DIR$conf{'dir_sep'}" . File::Spec->abs2rel($history{$idx}{$_}{'src'},$conf{'root_dir'}) 
                . "'  -->  'ROOT_DIR$conf{'dir_sep'}" . File::Spec->abs2rel($history{$idx}{$_}{'dst'},$conf{'root_dir'}) ."'\n");
      }
    }

    return $err;
  };

  my $set_dcc_bin_dir = sub {
    my $ccd_dir = shift(@_);
    my $dir_sep = shift(@_);

    my $is_ccd_valid = 0;
    my $dirh = undef;

    local $_;
    local $&;

    
    opendir($dirh, "$$ccd_dir") || do {
      printf STDERR "ERROR: fail to read directory contents of: '$history{$idx}{'bin'}{'dst'}'\n";
      return 1;
    };
    $_=join("\n", readdir($dirh));
    closedir($dirh);

    $is_ccd_valid = 0 + /^($history{$idx}{'bin'}{'pref'}.*)$/m;
    $$ccd_dir .= "${dir_sep}$1" if $is_ccd_valid;

    return $is_ccd_valid;
  };

  # Note ALL directories names are ABSOLUTE
  my @dirs = (find(@conf{'root_dir', 'dir_pattern', 'dir_sep'}, 0, ($DBG{'l1'} =~ m/$conf{'debug'}/)));

  while (defined($dir = pop @dirs)) {
    printf "\nEntering into: '${dir}'\n\n" if $DBG{'l1'} =~ m/$conf{'debug'}/;
    opendir($dirh, $dir) || do {
      printf STDERR "ERROR: fail to read directory contents of: '${dir}'\n";
      return 1;
    };

    ENTRY: while ( defined($cur = readdir($dirh)) ) {
      next ENTRY if ($cur =~ /^\.{1,2}$/);
      if ($cur =~ m/^.*$conf{'dcc_xml_pattern'}$/) {
        $idx++;
        $xml_id++;

        $history{$idx}{'xml'}{'abs'} =  "${dir}$conf{'dir_sep'}${cur}";
        $history{$idx}{'xml'}{'rel'} =  "$history{$idx}{'xml'}{'abs'}";
        $history{$idx}{'xml'}{'rel'} =~ s/\Q$conf{'root_dir'}\E\/?//;

        $history{$idx}{'dccgen_cmd'} =  "cd    \"" . dirname($history{$idx}{'xml'}{'abs'})  . "\""
                                        . " && \"" . $conf{'dccgen'}                        . "\""
                                        . "    \"" . basename($history{$idx}{'xml'}{'abs'}) . "\"";

        # Extract and assemble C, H and DCC file names, which corresponds to 
        # current XML file
        {
          unless (get_dcc_fnames(%{$history{$idx}}, $history{$idx}{'xml'}{'abs'}) == 0) {
            printf STDERR "ERROR: failed to read:\n  '$history{$idx}{'xml'}{'abs'}'\n\n";
            wait_user;
            next ENTRY;
          }
  
          $history{$idx}{'bin'}{'pref'} =  "cid$history{$idx}{'cid'}";
          $history{$idx}{'bin'}{'src'}  =  "$history{$idx}{'bin'}{'pref'}_" . basename($history{$idx}{'xml'}{'abs'});
          $history{$idx}{'bin'}{'src'}  =~ s/$conf{'dcc_xml_suffix'}$/$conf{'dcc_bin_suffix'}/;
  
          # Convert all file names to absolute
          $history{$idx}{$_}{'src'} = File::Spec->rel2abs("${dir}$conf{'dir_sep'}$history{$idx}{$_}{'src'}")
            for (@files);
        }
  
        # Execute DCC generator tool & save its stdout
        $history{$idx}{'cmd_out'} = join('', qx/$history{$idx}{'dccgen_cmd'}/);

        # Extract CRC sum
        $history{$idx}{'cmd_out'} =~ m/^\s*\[?\s*0x([[:xdigit:]]+)\s*\]?\s*$/mi;
        $history{$idx}{'crc'} = hex($1);
             
        # Check DCC generator exit status
        if ( $history{$idx}{'cmd_out'} eq '' ) {
          printf STDERR "ERROR: $conf{'dccgen'}: failed to execute or internal error occured for\n\n  '$history{$idx}{'xml'}{'abs'}'\n\n";
          printf STDERR "NOTE:  Verify that path to '$conf{'dccgen'}' is set in PATH evn or set absolute\n";
          printf STDERR "NOTE:  path to  DCC generator tool via --dccgen\n";
          wait_user;
          next ENTRY;
        }
        printf "$history{$idx}{'cmd_out'}" if $DBG{'cmd_out'} =~ m/$conf{'debug'}/;
        printf "0x%08X : '%s'\n", $history{$idx}{'crc'}, $history{$idx}{'xml'}{'rel'}
          if $DBG{'l2'} =~ m/$conf{'debug'}/;

        # First XML file from a given dir should not be compared,
        # as there is no previous CRC
        if ( ($xml_id > 1) && ($history{$idx-1}{'crc'} != $history{$idx}{'crc'}) ) {
          printf STDERR "ERROR: these two XMLs trigger calculating different CRCs:\n";
          printf STDERR "  CRC=0x%08X, FILE='%s'\n"  , $history{$idx-1}{'crc'}, $history{$idx-1}{'xml'}{'abs'};
          printf STDERR "  CRC=0x%08X, FILE='%s'\n\n", $history{$idx}{'crc'}  , $history{$idx}{'xml'}{'abs'};
          wait_user;
          &$cleanup( @files );
          next ENTRY;
        }

        # Extract install paths of C, H and DCC files    
        { 
          unless (get_install_paths(%{$history{$idx}}, "${dir}$conf{'dir_sep'}$conf{'dcc_result_location'}") == 0) {
            printf STDERR "ERROR: can't find, where to to copy C, H and DCC files.\n";
            printf STDERR "ERROR: file with locations DOESN'T exists or NOT valid:\n\n";
            printf STDERR "  '${dir}$conf{'dir_sep'}$conf{'dcc_result_location'}'\n\n";
            wait_user;
            &$cleanup( @files );
            next ENTRY;
          }

          # Convert all file names to absolute
          $history{$idx}{$_}{'dst'} = File::Spec->rel2abs("$conf{'root_dir'}$conf{'dir_sep'}$history{$idx}{$_}{'dst'}")
            foreach (@files);
        }
        # bin file are copied into subfolder with prefix cid<CameraID>, 
        # append this to the dst path for DCC bin file
        $history{$idx}{'bin'}{'valid'} = &$set_dcc_bin_dir(\$history{$idx}{'bin'}{'dst'}, $conf{'dir_sep'});


        print_hash('  ', "\n\nHISTORY: ENTRY $idx", %{$history{$idx}})
          if $DBG{'print_history'} =~ m/$conf{'debug'}/;

        # Install files generated by DCC generator        
        {          
          unless ($history{$idx}{'bin'}{'valid'}) {
            printf STDERR "ERROR: invalid sensor ID: '$history{$idx}{'cid'}'\n";
            printf STDERR "ERROR: file: '$history{$idx}{'bin'}{src}'\n";
            wait_user;
            #&$cleanup('bin');
            &$cleanup(@files);
            next ENTRY;
          }
          if ( &$install(@files) != 0 ) { 
            wait_user; 
          }
        }
      } else {  
          printf "Skipping %s: '$cur'\n\n", (-d "${dir}$conf{'dir_sep'}$cur" ? "dir": "file" ) 
            if $DBG{'l1'} =~ m/$conf{'debug'}/;
      }   
    } 
    $xml_id = 0;
    closedir dirh;
  }
}

sub find  {
  my ($dir, $dir_pattern, $dir_sep, $chomp, $verbose) = (@_);
  my $abs = undef;  
  my $cur = undef ;
  my $dirh = undef;
  my @dirs = ();
  my @dcc_dirs = ();

  $dir_pattern = qr|^(.*\Q${dir_sep}\E)?${dir_pattern}[^\Q${dir_sep}\E]*?(\Q${dir_sep}\E\s*)?$|;

  slashes($dir, 0);
  # remove any trailing spaces in directory name
  $dir =~ s/\s*$// if $chomp;
  push(@dirs, $dir);
  
  push(@dcc_dirs, $dir) if ($dir =~ m/$dir_pattern/);

  DIR: while (defined($dir = pop(@dirs))) {
    printf "Entering into: '${dir}'\n\n" if $verbose;
    opendir($dirh, $dir) || do {
      printf STDERR "ERROR: fail to read directory contents of: '${dir}'\n";
      next DIR;
    };

    ENTRY: while ( defined($cur = readdir($dirh)) ) {
      # Skip '.'  and '..' directories
      next ENTRY if ($cur =~ /^\.{1,2}$/);
      
      # Convert every pathname to absolute pathname    
      $abs="${dir}${dir_sep}$cur";
      
      if ( -d $abs ) {
        if ($abs =~ m/$dir_pattern/) { 
          printf "QUEUE: '$abs'\n\n" if $verbose; 
          push (@dcc_dirs, $abs); 
          # next ENTRY;
        }
        push (@dirs, $abs); 
      }
    } 
    closedir dirh;
  }
  
  return wantarray ? @dcc_dirs : $#dcc_dirs;
}

###########################################################
# MAIN
###########################################################

if ($#ARGV <= -1) { 
  print "$USAGE"; 
  exit 0; 
}

set_defaults(@ARGV);
parse_cmd(@ARGV);
verify_config();

$CONF{'dir_pattern'}     = quotemeta($CONF{'dir_pattern'});
$CONF{'dcc_xml_pattern'} = quotemeta($CONF{'dcc_xml_pattern'});

print_hash(' ', "USING CONFIG", %CONF);
wait_user("\nVerify that the above CONFIG is correct."
          . "\nPress [ENTER] to continue ... "
          . "\nPress [CTRL+C] to exit ... \n\n");

if ($CONF{'list'}) {
  print( "List of directories, which shall be proccessed:\n\n  ",
        join "\n  ", find( @CONF{'root_dir', 'dir_pattern', 'dir_sep'} ) );
  exit 0;
}

dccgen_tree(%CONF);


__END__
