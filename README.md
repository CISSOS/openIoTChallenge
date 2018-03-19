# CISSOS Open IoT Challenge Project

The projects in this repository were created by the CISSOS team for the Open IoT Challenge hosted by eclipse IoT. For further background information, please feel free to find our blog on medium:
https://medium.com/@cissos/

This file guides you through setting up all the different components within our architecture.
Please note, that this repository and all the included source code is being provided privately by the CISSOS-Team members and we do not provide any warranty.
![Final architecture](https://github.com/CISSOS/openIoTChallenge/blob/master/resources/Final_architecture.png?raw=true)
## Hardware requirements:
- Raspberry Pi (we used version 3)
- Bosch XDK

# Kura
 - Download and install Kura on the Raspberry Pi as described in : https://eclipse.github.io/kura/intro/raspberry-pi-quick-start.html
 - Log onto the Kura web console and make sure you have the following configuration:
	 - Network settings - 
		 - eth0
			 - Change "Configure" to "Manually" and assign a static IP address to your device
		 - wlan0
			 - TCP/IP
				 - Status: Enabled for LAN
				 - Configure: Manually
				 - IP Address: <your_ip>
				 - Subnet Mask: 255.255.255.0
			- Wireless
				- Wireless mode: Access point
				- Network name: <your_network_name>
				- Wireless password: <your_wifi_code>
			- DHCP & NAT
				- Router mode: DHCP and NAT
				- DHCP Subnet mask: 255.255.255.0
	- Simple Artemis MQTT Broker
		- Enabled = true
		- MQTT address = 0.0.0.0
		- MQTT broker port = 1883
		- User name = blank
		- Password of the user =blank
	- Firewall
		- Create entry for Port 1883 | tcp | 0.0.0.0/0 | eth0 
		-  Create entry for Port 1883 | tcp | 0.0.0.0/0 | wlan0
	- Go to packages, click install and browse to the .dp-file, that is located under de.bosch.com.cissos.mqttprocess/resources/dp
	- Refresh the web console and you should see a new service called "CISSOS JSON Processor" - here you can change the channels, that the processor is reading from and writing to.

# XDK
1. Download and install the XDK Workbench Version 2.0.1 as described here: https://xdk.bosch-connectivity.com/de/software-downloads
2. Open the XDK Worbench and import the project from the folder /XDK_Application/Bosch-XDK-Portal-orig (please note, that this project is just a slightly modified version of a project, that is delivered with the XDK-Workbench by Bosch Connected Devices and Solutions!)
3. Open the file /source/protected/BCDS_bxpConfig.h and change the network settings to the network, that you just created on Kura (Network Settings -> wlan0 -> wireless)
4. Save the file
5. Build the project (Project -> Build all or by pressing Ctrl+B)
6. Connect your XDK device via USB and flash the binary to your device
7. During flashing and startup of the application, check the Workbench-console to make sure, there are no errors
8. Once the application has started, the XDK can be disconnected and will reconnect to the WiFi network and the Broker, whenever it gets turned on and publish data to the topic "stream" -> Use the '+' sign as a  wildcard to subscribe (e.g. "+/+/+/+/+/stream") 

# Ditto and Twin setup
1. Setup Ditto as described here: https://www.eclipse.org/ditto/intro-overview.html
2. A thing (digital twin) is a generic entity which is used to cluster multiple features and manage the access to the data and functionality of the thing.
3. Create a thing by posting PUT-request to your Ditto-API using the JSON-file provided under /Data_Model as a body

# Kura-to-ditto-service
 Make sure, that this service runs on a machine in the same network as the Kura's ethernet connection e.g. on a local PC
1. Download, install and run  XAMPP as described here: https://www.apachefriends.org/de/download.html
2. Open the javascript file under /Kura-to-ditto-service in an editor of your choice and change the configuration according to the comments within the file
3. Start Apache and Tomcat in XAMPP
4. Copy your files into a new folder within the tomcat directory
5. Open you website in a browser http://localhost:8080/<YOURNEWFOLDER>/webservices.html
 -> Note that the website needs to remain opened as long as you want the data connection between Kura and Ditto to stay established
# Data Analytics service
1. If not already done, start Apache, Tomcat and MySQL in XAMPP
2. Go to localhost/phpmyadmin in your web browser and create a new database
3. Your database needs one table to store the sensor data
4. You can use the SQL dump file provided under /Data_Analytics_service (--> copy and paste the SQL statements to create the database and the table)
5. Configure the python script provided under /DataAnalyticsService according to the comments in the file
6. pip install the requests library: http://docs.python-requests.org/en/master/
7. Run the python script
# Web UI
1. Copy the HTML, CSS and JS files provided in /Web_UI into a new folder in your tomcat/webapps directory
2. Modify the JS-File to your specific settings:
--> You have to specify 3 things:
-   The URL of your thing
-   Your Basic Authorization (Base 64 encoded Basic Auth)
-   Your x-cr-api-token
3. Apache, Tomcat and MySQL must be running
4. Execute the Python script
Start the website of the Web UI:  
http://localhost:8080/ui-folder/sensordata.html

Please note, that you must start the webservice and the Web UI from http://localhost:8080 as HTTP requests from http://127.0.0.1:8080/ will fail!
# Twitterbot
1. Create a Twitter account and your api information as described here:
https://dototot.com/how-to-write-a-twitter-bot-with-python-and-tweepy/
2. pip install the tweepy library: https://github.com/tweepy/tweepy
3. Modify the python script provided under /Twitter_Bot according to the comments within the file
4. Execute the python script
