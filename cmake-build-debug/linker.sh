unlink rootfile
unlink convfile

if [ $1 = "0" ]; then
    echo "unlink rootfile"
    echo "unlink convfile"
else
    ln -s ../../rootfile ./rootfile
    ln -s ../../rootfile ./convfile
    echo "ln -s ../../rootfile ./rootfile"
    echo "ln -s ../../rootfile ./convfile"
fi