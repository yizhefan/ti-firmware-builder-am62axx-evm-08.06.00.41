#  Build Environment Setup {#ENVIRONMENT_SETUP}

[TOC]

# IMPORTANT NOTES
- <b> Make sure you don't skip any of the non-optional steps mentioned below </b>
- ${PSDKR_PATH} refers to the path where Processor SDK RTOS (PSDK RTOS) is installed
- ${PSDKL_PATH} refers to the path where Processor SDK Linux (PSDK Linux) is installed
- All folders like, pdk, tiovx, vision_apps mentioned in the user guide are relative to ${PSDKR_PATH} unless explicitly stated otherwise.
- The build is tested on Ubuntu (x86_64) 18.04 system and may not work on earlier or later Ubuntu systems.
- 20GB of free space is required to install and build PSDK RTOS
- Make sure you have sudo access

# Step 1a: Download and Install PSDK RTOS

- Download and untar PSDK RTOS Tarball **ti-processor-sdk-rtos-${SOC}-evm-xx_xx_xx_xx.tar.gz** on a Ubuntu 18.04 (x86_64) machine.
\code
 cd {path/to/file}
 tar xf ti-processor-sdk-rtos-${SOC}-evm-xx_xx_xx_xx.tar.gz
\endcode

# Step 1b: Download and Install PSDK RTOS Add-on to run in PC Emulation Mode (Optional, only needed for PC emulation mode)

- Download and Run PSDK RTOS Add-on package installer file **ti-processor-sdk-rtos-${SOC}-evm-xx_xx_xx_xx-linux-x64-installer.run** on a Ubuntu 18.04 (x86_64). This package
  contains performance datasheet documentation as well as PC compiled cmodel libraries for VPAC and DMPAC OpenVX nodes.
  \attention The installer is available only through TI mySecure login.
  \attention To Request access for PSDK RTOS Add-on Packages the first time: [<a href="https://www.ti.com/licreg/docs/swlicexportcontrol.tsp?form_id=276074&prod_no=PSDK-RTOS-AUTO&ref_url=adas">LINK</a>]
  \attention mySecure SW Access Link (after access has been granted) : [<a href="https://www.ti.com/securesoftware/docs/securesoftwarehome.tsp">LINK</a>]
  \note This mySecureSW site also contains MCUSW AUTOSAR Configurator package installer
- Follow instructions on screen to complete the installation.
  \note The Installation Directory should be given as the parent directory of ${PSDKR_PATH} (If you choose the Browse option, Make sure you are NOT creating a new folder in the parent directory)

# Step 2a: Download and install PSDK Linux (Needed for Linux+RTOS mode or for QNX SPL mode) {#ENVIRONMENT_SETUP_STEP2}

- PSDK RTOS needs PSDK Linux to build. Run the installer file **ti-processor-sdk-linux-${SOC}-evm-xx_xx_xx_xx-Linux-x86-Install.bin**
on the same Ubuntu 18.04 (x86_64) machine.
- Follow instructions on screen to complete the installation
- Copy below files (linux filesystem and linux boot files) from PSDK Linux folder to base folder of PSDK RTOS
\code
 cp ${PSDKL_PATH}/board-support/prebuilt-images/boot-${SOC}-evm.tar.gz ${PSDKR_PATH}/
 cp ${PSDKL_PATH}/filesystem/tisdk-edgeai-image-${SOC}-evm.tar.xz ${PSDKR_PATH}/
\endcode

# Step 2b: Download and Install PSDK QNX addon to run in QNX+RTOS mode (Optional, only needed for QNX+RTOS mode) {#ENVIRONMENT_SETUP_QNX}

  The PSDK QNX addon package is dependent on installation of packages from both QNX and Texas Instruments. Reference PSDK QNX Documentation on Getting Started, for details on download and installation.

  Once PSDK QNX download and installation is complete, proceed with below steps.

# Step 3: Proxy Setup {#PROXY_SETUP}

**Skip this step if you are not using a proxy server**

In order to download and compile some components from next step, it is required that your
PC network proxy is setup properly. Below instructions show how
the network proxy is setup assuming a proxy server named "http://proxy.company.com:80"
for the domain "company.com". Modify these appropriately for your network.

## Proxy for wget

- To set the "wget" proxy add below lines in ~/.wgetrc
    \code
    http_proxy=http://proxy.company.com:80
    https_proxy=http://proxy.company.com:80
    ftp_proxy=http://proxy.company.com:80
    noproxy=ti.com
    \endcode

## Proxy for http access

- Setup HTTP proxy by adding below lines in your ~/.bashrc
    \code
    export HTTPS_PROXY=http://proxy.company.com:80
    export https_proxy=http://proxy.company.com:80
    export HTTP_PROXY=http://proxy.company.com:80
    export http_proxy=http://proxy.company.com:80
    export ftp_proxy=http://proxy.company.com:80
    export FTP_PROXY=http://proxy.company.com:80
    export no_proxy=ti.com
    \endcode

## Proxy for "apt-get"

- Add below line in the file /etc/apt/apt.conf
    \code
    Acquire::http::proxy "http://proxy.company.com:80";
    \endcode

## Proxy for "git"

- Install corkscrew
    \code
    sudo apt-get install corkscrew
    \endcode

- Create a file ~/.gitconfig and add below content to it
    \code
    [user]
            email = <myemail>@company.com
            name = <myname>
    [core]
            gitproxy = none for company.com
            gitproxy = /home/<username>/git-proxy.sh
    [http]
            proxy = http://proxy.company.com:80
    [https]
            proxy = http://proxy.company.com:80
    \endcode

- Create a file ~/git-proxy.sh and add below lines in it
    \code
    #!/bin/sh
    exec /usr/bin/corkscrew proxy.company.com 80 $*
    \endcode

- Make ~/git-proxy.sh as executable
    \code
    chmod +x ~/git-proxy.sh
    \endcode

## Proxy for "curl"

- Add below line in ~/.curlrc file
    \code
    proxy=http://proxy.company.com:80
    \endcode

# Step 4: Download and install additional dependencies {#ADDITIONAL_DOWNLOADS}

\note 'sudo' permission is required to run below script and install some components using apt-get
      In case you don't want to install packages with sudo permission pass below argument to this script
      \code
      --skip_sudo
      \endcode
      Open below setup_psdk_rtos file and install the packages installed with **sudo apt install** separately

\note In case you are only using QNX SBL in your development, you can skip any SPL-related installations
      by passing the below flag when running the setup_psdk_rtos script.
      \code
      --qnx_sbl
      \endcode

- Do below to download and install additional dependencies needed to build PSDK RTOS
  \code
  cd ${PSDKR_PATH}
  ./psdk_rtos/scripts/setup_psdk_rtos.sh
  \endcode
  \note Make sure to call the script from ${PSDKR_PATH} as shown above. DO NOT "cd" into psdk_rtos/scripts and call the script

