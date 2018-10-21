unlink rootfile
unlink convfile

if [ $1 = "0" ]; then
    echo "rootfile and convfile unlinked\n";
else
    ln -s ../../rootfile ./rootfile
    ln -s ../../rootfile ./convfile
    echo "rootfile and convfile linked\n";
fi