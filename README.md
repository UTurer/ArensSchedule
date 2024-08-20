This is a one week project I did. The idea came from the schedule for my 7th grade kid being too complex to track. He had to cross-reference 4 different tables to see where he had to be at that hour. Furthermore, he likes tracking how many minutes are left to the end of the course. So the idea that we can utilize the Samsung Galaxy Watch 3 sitting in a drawer in our house for this task. I had to learn Tizen Studio and Native API throughout this one week, but I managed to code a working app.

Reminders to Self:
1) In order to connect and run the app on the actual watch hardware (not the emulator that comes with Tizen Studio) you have to do as follows:
   a) The following two links are a great start:
       - https://developer.samsung.com/galaxy-watch-tizen/creating-your-first-app/native.html
       - https://developer.samsung.com/sdp/blog/en/2021/04/12/remote-device-manager-an-easy-way-to-launch-your-application-with-tizen-studio
   b) On the watch, Set WiFi to "Always On" instead of "Auto".
   c) On the watch, turn Bluetooth off. In my first try I didn't have to turn it off to connect, but after I synced the watch to another phone, I had to turn off Bluetooth to connect so if the watch does not connect to Tizen Studio, it may be because of bluetooth being turned on.
   c) On the watch, activate developer mode. Search internet for the steps but here is a reminder which I hope I remember correctly:
       - Go to "Settings->About Watch->Software". Click on "Software Version" button 5 times. A notification will appear saying that developer mode is now on.
       - Restart the watch.
       - After restart, go to "Settings->About Watch. Scroll to the bottom and find "Debugging is turned off" text. Click on it to turn on debugging.
   c) The watch connects to 2.4GHz WiFi. My laptop connects to 5GHz on the same router. I had to connect the laptop to 2.4GHz WiFi also.
   d) Connect to the watch using Tizen Studio Device Manager
       - Open Tizen Studio and launch Device Manager (Alt+Shift+V or go to Tools->Device Manager).
       - Click "Scan" button. The watch will hopefully be found. You can check the IP address from the WiFi menu of the watch.
       - Slide Connection from OFF to ON. A notification appears on the watch screen. If the watch is on standby, you have to wake it up to see the notification. The notification is about "Debugging RSA Key Fingerprint ... etc.". Click OK and close the notification.
       - Hopefuly after this step the watch will connect to Tizen Studio.
   e) Now you can choose the watch as the target hardware while running the code.
2) In order to build a package, I had to create a certificate and sign the app. This is how I did it:
   a) The following link is a great start
       - https://developer.samsung.com/tizen/certificate-signing/creating-certificate.html
   b) I followed the steps in the link. I opened Tizen Studio Package Manager and downloaded Samsung Certificate Extension.
   c) Then inside Tizen Studio I opened Certificate Manager. It showed me two options Tizen or Samsung. I selected Samsung because it was a wearable device app.
   d) I created an Author Certificate. Wrote my name, password. Selected "use the same password for distributor certificate".
   e) It opened a webpage and asked me to login to my Samsung account. I logged in. It created the Author Certificate.
   f) Then it continued with the Distributor Certificate. Somewhere it asked for a DUID. Since the watch was already connected to Tizen Studio, it automatically selected the required DUID, I just clicked Next.
