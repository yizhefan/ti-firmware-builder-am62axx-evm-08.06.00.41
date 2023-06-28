grep -R -l "tivxMemShared2TargetPtr" . | xargs perl -i -pe 'BEGIN{undef $/;} s/tivxMemShared2TargetPtr\(\s*?(\S*?)\.shared_ptr,.*?mem_heap_region\);/tivxMemShared2TargetPtr\(&$1\);/smg'
