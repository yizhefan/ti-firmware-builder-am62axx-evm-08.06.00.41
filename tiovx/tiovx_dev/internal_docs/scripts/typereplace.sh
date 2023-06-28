grep -R -l "U08" . | xargs sed -i 's/ U08 / uint8_t /g'
grep -R -l "U08" . | xargs sed -i 's/(U08/(uint8_t/g'
grep -R -l "U08" . | xargs sed -i 's/*U08/*uint8_t/g'

grep -R -l "U16" . | xargs sed -i 's/ U16 / uint16_t /g'
grep -R -l "U16" . | xargs sed -i 's/(U16/(uint16_t/g'
grep -R -l "U16" . | xargs sed -i 's/*U16/*uint16_t/g'

grep -R -l "U32" . | xargs sed -i 's/ U32 / uint32_t /g'
grep -R -l "U32" . | xargs sed -i 's/(U32/(uint32_t/g'
grep -R -l "U32" . | xargs sed -i 's/*U32/*uint32_t/g'

grep -R -l "U64" . | xargs sed -i 's/ U64 / uint64_t /g'
grep -R -l "U64" . | xargs sed -i 's/(U64/(uint64_t/g'
grep -R -l "U64" . | xargs sed -i 's/*U64/*uint64_t/g'


grep -R -l "S08" . | xargs sed -i 's/ S08 / int8_t /g'
grep -R -l "S08" . | xargs sed -i 's/(S08/(int8_t/g'
grep -R -l "S08" . | xargs sed -i 's/*S08/*int8_t/g'

grep -R -l "S16" . | xargs sed -i 's/ S16 / int16_t /g'
grep -R -l "S16" . | xargs sed -i 's/(S16/(int16_t/g'
grep -R -l "S16" . | xargs sed -i 's/*S16/*int16_t/g'

grep -R -l "S32" . | xargs sed -i 's/ S32 / int32_t /g'
grep -R -l "S32" . | xargs sed -i 's/(S32/(int32_t/g'
grep -R -l "S32" . | xargs sed -i 's/*S32/*int32_t/g'

grep -R -l "S64" . | xargs sed -i 's/ S64 / int64_t /g'
grep -R -l "S64" . | xargs sed -i 's/(S64/(int64_t/g'
grep -R -l "S64" . | xargs sed -i 's/*S64/*int64_t/g'

grep -R -l "U08" . | xargs sed -i 's/^U08/uint8_t/g'
grep -R -l "U16" . | xargs sed -i 's/^U16/uint16_t/g'
grep -R -l "U32" . | xargs sed -i 's/^U32/uint32_t/g'
grep -R -l "U64" . | xargs sed -i 's/^U64/uint64_t/g'
grep -R -l "S08" . | xargs sed -i 's/^S08/int8_t/g'
grep -R -l "S16" . | xargs sed -i 's/^S16/int16_t/g'
grep -R -l "S32" . | xargs sed -i 's/^S32/int32_t/g'
grep -R -l "S64" . | xargs sed -i 's/^S64/int64_t/g'


grep -R -l "D64" . | xargs sed -i 's/ D64 / VLIB_D64 /g'
grep -R -l "D64" . | xargs sed -i 's/(D64/(VLIB_D64/g'
grep -R -l "D64" . | xargs sed -i 's/*D64/*VLIB_D64/g'
grep -R -l "D64" . | xargs sed -i 's/^D64/VLIB_D64/g'

grep -R -l "F32" . | xargs sed -i 's/ VLIB_F32 / VLIB_F32 /g'
grep -R -l "F32" . | xargs sed -i 's/(VLIB_F32/(VLIB_F32/g'
grep -R -l "F32" . | xargs sed -i 's/*VLIB_F32/*VLIB_F32/g'
grep -R -l "F32" . | xargs sed -i 's/^F32/VLIB_F32/g'

grep -R -l "PTR" . | xargs sed -i 's/ VLIB_PTR / VLIB_PTR /g'
grep -R -l "PTR" . | xargs sed -i 's/(VLIB_PTR/(VLIB_PTR/g'
grep -R -l "PTR" . | xargs sed -i 's/*VLIB_PTR/*VLIB_PTR/g'
grep -R -l "PTR" . | xargs sed -i 's/^PTR/VLIB_PTR/g'

grep -R -l "VAL" . | xargs sed -i 's/ VLIB_VAL / VLIB_VAL /g'
grep -R -l "VAL" . | xargs sed -i 's/(VLIB_VAL/(VLIB_VAL/g'
grep -R -l "VAL" . | xargs sed -i 's/*VLIB_VAL/*VLIB_VAL/g'
grep -R -l "VAL" . | xargs sed -i 's/^VAL/VLIB_VAL/g'
