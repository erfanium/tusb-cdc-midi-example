set -e;

cd build;
cmake ..;
make;
cp ./*.uf2 /media/erfanium/RPI-RP2;
echo "Deployed!"

# wait for 5 seconds
sleep 5;

cat /dev/ttyACM0;
