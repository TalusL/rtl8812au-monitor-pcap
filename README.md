# rtl8812au-monitor-pcap
Use RTL8812AU to capture 802.11 raw data frame and dump to .pcap file on Windows

### rtl8812au userspace driver
[devourer](https://github.com/openipc/devourer)

The RTL8812AU driver that simply devours its competitors

### Windows

You can download and install libraries using the vcpkg dependency manager:

```
cd rtl8812au-monitor-pcap
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
.\vcpkg install libusb libpcap
cd ..
cmake -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake  -G "Visual Studio 16 2019" -A x64 -S ./ -B "build64" 
cmake --build build64 --config Release --target WiFiCapture
```
