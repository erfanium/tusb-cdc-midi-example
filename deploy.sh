set -e;

cd build;
cmake ..;
make;
cp ./*.uf2 /media/$USER/RPI-RP2;
echo "Deployed!"

# wait for 5 seconds
sleep 5;

cat /dev/ttyACM0;
