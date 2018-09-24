RUNID=1030
NEVENT=10

for((i=0;i<$NEVENT;++i))
do
    \root -l -b -q 'wfm.C('$RUNID', '$i')'
done