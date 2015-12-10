# Update and dependencies
apt-get update
apt-get install -y python openjdk-7-jdk ant unzip

# Android SDK
cd /home/vagrant
wget /home/vagrant http://dl.google.com/android/android-sdk_r24.4.1-linux.tgz
tar xf android-sdk_r24.4.1-linux.tgz
cd android-sdk-linux/tools
./android update sdk -t 19 --no-ui

echo "export ANDROID_SDK_ROOT=/home/vagrant/android-sdk-linux" >> /home/vagrant/.bashrc

# Android NDK
cd /home/vagrant
wget http://dl.google.com/android/ndk/android-ndk-r10e-linux-x86.bin
chmod +x android-ndk-r10e-linux-x86.bin
./android-ndk-r10e-linux-x86.bin

echo "export NDK_ROOT=/home/vagrant/android-ndk-r10e" >> /home/vagrant/.bashrc

source .bashrc

# cocos2d-x
wget http://www.cocos2d-x.org/filedown/cocos2d-x-3.9.zip
mkdir cocos2d-x
unzip cocos2d-x-3.9.zip -d cocos2d-x
cd cocos2d-x
python setup.py
