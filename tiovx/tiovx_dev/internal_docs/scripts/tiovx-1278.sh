grep -R -l "ownIsValidSpecificReference" . | xargs sed -i 's/ownIsValidSpecificReference(&\(.*\)->base/ownIsValidSpecificReference((vx_reference)\1/g'
grep -R -l "ownIsValidSpecificReference" . | xargs sed -i 's/ownIsValidSpecificReference( &\(.*\)->base/ownIsValidSpecificReference((vx_reference)\1/g'
