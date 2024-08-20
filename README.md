This is a one week project I did. The idea came from the schedule for my 7th grade kid being too complex to track. He had to cross-reference 4 different tables to see where he had to be at that hour. Furthermore, he likes tracking how many minutes are left to the end of the course. So the idea that we can utilize the Samsung Galaxy Watch 3 sitting in a drawer in our house for this task came to me. I had to learn Tizen Studio and Native API throughout this one week, but I managed to code a working app.

The app basically has a schedule for weekdays and weekends. When you open the app, it shows the schedule for that day, then according to the current time, it fills progressbars accordingly, marks active course with a yellow color and marks completed courses with dark gray etc.

Reminders to Self:
1) In order to connect and run the app on the actual watch hardware (not the emulator that comes with Tizen Studio) you have to do as follows:<br/>
   a) The following two links are a great start:<br/>
       - https://developer.samsung.com/galaxy-watch-tizen/creating-your-first-app/native.html<br/>
       - https://developer.samsung.com/sdp/blog/en/2021/04/12/remote-device-manager-an-easy-way-to-launch-your-application-with-tizen-studio<br/>
   b) On the watch, Set WiFi to "Always On" instead of "Auto".<br/>
   c) On the watch, turn Bluetooth off. In my first try I didn't have to turn it off to connect, but after I synced the watch to another phone, I had to turn off Bluetooth to connect so if the watch does not connect to Tizen Studio, it may be because of bluetooth being turned on.<br/>
   c) On the watch, activate developer mode. Search internet for the steps but here is a reminder which I hope I remember correctly:<br/>
       - Go to "Settings->About Watch->Software". Click on "Software Version" button 5 times. A notification will appear saying that developer mode is now on.<br/>
       - Restart the watch.<br/>
       - After restart, go to "Settings->About Watch. Scroll to the bottom and find "Debugging is turned off" text. Click on it to turn on debugging.<br/>
   c) The watch connects to 2.4GHz WiFi. My laptop connects to 5GHz on the same router. I had to connect the laptop to 2.4GHz WiFi also.<br/>
   d) Connect to the watch using Tizen Studio Device Manager<br/>
       - Open Tizen Studio and launch Device Manager (Alt+Shift+V or go to Tools->Device Manager).<br/>
       - Click "Scan" button. The watch will hopefully be found. You can check the IP address from the WiFi menu of the watch.<br/>
       - Slide Connection from OFF to ON. A notification appears on the watch screen. If the watch is on standby, you have to wake it up to see the notification. The notification is about "Debugging RSA Key Fingerprint ... etc.". Click OK and close the notification.<br/>
       - Hopefuly after this step the watch will connect to Tizen Studio.<br/>
   e) Now you can choose the watch as the target hardware while running the code.<br/>
2) In order to build a package, I had to create a certificate and sign the app. This is how I did it:<br/>
   a) The following link is a great start<br/>
       - https://developer.samsung.com/tizen/certificate-signing/creating-certificate.html<br/>
   b) I followed the steps in the link. I opened Tizen Studio Package Manager and downloaded Samsung Certificate Extension.<br/>
   c) Then inside Tizen Studio I opened Certificate Manager. It showed me two options Tizen or Samsung. I selected Samsung because it was a wearable device app.<br/>
   d) I created an Author Certificate. Wrote my name, password. Selected "use the same password for distributor certificate".<br/>
   e) It opened a webpage and asked me to login to my Samsung account. I logged in. It created the Author Certificate.<br/>
   f) Then it continued with the Distributor Certificate. Somewhere it asked for a DUID. Since the watch was already connected to Tizen Studio, it automatically selected the required DUID, I just clicked Next.<br/>
<br/>
TO-DO LIST<br/>
[ ] App currently has a timer with 10 sec interval checking for changes. It may be better if the app, while active, would listen for clock changes and if minute or hour is changed, update the schedule progress. However you have to find an event that fires every minute. If it fires every second or milisecond, it will probably be more cpu intensive?<br/>
[ ] Currently when schedule is being updated, all labels and progressbars are being deleted and then readded. Instead, there could be a smart mechanism which checks for which labels and progressbars to update, and only update those.<br/>
[ ] Schedule items can be made collapsible. Currently too much info is shown on the screen and you have to scroll a lot. Maybe you can collapse the individual items and see only the progress bars. And when you click on a progress bar, more text info appears.<br/>
[ ] You can add another window which can be reached with a swipe to the left or right. It can be the options screen where you define colors, text size etc.<br/>
[ ] You can add another window which can ve reached with a swipte to the left or right. It can be for entering a schedule. Currently the schedule is coded as a struct inside the code and it has to be changed inside the code and re-compiled. If this app is going to be published, it has to be done inside the app.<br/>
