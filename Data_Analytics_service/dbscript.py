import MySQLdb
import requests
from time import sleep
from datetime import datetime

# ! The URL of the Thing
thingUrl = "https://Url-to-Ditto.com/api/2/things/come.your.namespace:YOURTHING/"

# ! The Basic Authorization string
authorization = "Basic xxxxxxxxxxxxxxxxxx"

# ! The cr-api-token
apiToken = "xxxxxxxxxxxxxxxxxxxxx"

# DB Connection
db = MySQLdb.connect(host="localhost",    # your host (localhost)
                     user="root",         # your username, try root
                     passwd="",  # your password (or an empty string)
                     db="twindb")        # name of your database

# Create a cursor object to execute SQL queries 
cur = db.cursor()


# Infinite while loop to make HTTP requests 

while 1 == 1 :

    # HTTP GET Request
    rg = requests.get(thingUrl,
                     headers={'Authorization': authorization,
                              "Content-Type": "application/json",
                              "x-cr-api-token": apiToken});

    # Deserialize JSON
    twin = rg.json()

    # Current time for timestamp
    timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    print(timestamp)
    

    # Save sensor data in variables

    temp = twin['features']['temperature']['properties']['value']
    humidity = twin['features']['humidity']['properties']['value']
    pressure = twin['features']['pressure']['properties']['value']
    light = twin['features']['light']['properties']['value']

    mag_x = twin['features']['mag']['properties']['x']
    mag_y = twin['features']['mag']['properties']['y']
    mag_z = twin['features']['mag']['properties']['z']

    acc_x = twin['features']['acc']['properties']['x']
    acc_y = twin['features']['acc']['properties']['y']
    acc_z = twin['features']['acc']['properties']['z']

    gyro_x = twin['features']['gyro']['properties']['x']
    gyro_y = twin['features']['gyro']['properties']['y']
    gyro_z = twin['features']['gyro']['properties']['z']


    # Insert values into DB

    cur.execute("INSERT INTO sensordata (thing_id, timestamp, temperature, humidity, pressure, light, mag_x, mag_y, mag_z, acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z) "
                +"VALUES (1, '"+timestamp+"', "+temp+", "+humidity+", "+pressure+", "+light+", "+mag_x+", "+mag_y+", "+mag_z+", "+acc_x+", "+acc_y+", "+acc_z+", "+gyro_x+", "+gyro_y+", "+gyro_z+")")

    # Important: Insert-Statements must be commited
    db.commit()

    # Determine minimum, maximum and average values for temperature, humidity, pressure and light
    5
    cur.execute('SELECT MAX(temperature) FROM sensordata WHERE thing_id=1')

    # print all the first cell of all the rows
    for row in cur.fetchall() :
        max_temp = (row[0])

    cur.execute('SELECT MIN(temperature) FROM sensordata WHERE thing_id=1')

    for row in cur.fetchall() :
        min_temp = (row[0])
        

    cur.execute('SELECT AVG(temperature) FROM sensordata AS averagetemp WHERE thing_id=1')

    for row in cur.fetchall() :
        avg_temp = int(row[0])


    cur.execute('SELECT MAX(humidity) FROM sensordata WHERE thing_id=1')

    # print all the first cell of all the rows
    for row in cur.fetchall() :
        max_humidity = (row[0])
        #print(max_humidity)


    cur.execute('SELECT MIN(humidity) FROM sensordata WHERE thing_id=1')

    for row in cur.fetchall() :
        min_humidity = (row[0])


    cur.execute('SELECT AVG(humidity) FROM sensordata AS averagetemp WHERE thing_id=1')

    for row in cur.fetchall() :
        avg_humidity = int(row[0])
        

    cur.execute('SELECT MAX(pressure) FROM sensordata WHERE thing_id=1')

    # print all the first cell of all the rows
    for row in cur.fetchall() :
        max_pressure = (row[0])


    cur.execute('SELECT MIN(pressure) FROM sensordata WHERE thing_id=1')

    for row in cur.fetchall() :
        min_pressure = (row[0])


    cur.execute('SELECT AVG(pressure) FROM sensordata AS averagetemp WHERE thing_id=1')

    for row in cur.fetchall() :
        avg_pressure = int(row[0])


    cur.execute('SELECT MAX(light) FROM sensordata WHERE thing_id=1')

    # print all the first cell of all the rows
    for row in cur.fetchall() :
        max_light = (row[0])
        

    cur.execute('SELECT MIN(light) FROM sensordata WHERE thing_id=1')

    for row in cur.fetchall() :
        min_light = (row[0])


    cur.execute('SELECT AVG(light) FROM sensordata AS averagetemp WHERE thing_id=1')

    for row in cur.fetchall() :
        avg_light = int(row[0])


    # Load query results into thing

    # Encode values to send them via HTTP
    max_temp = str(max_temp).encode('utf-8')
    min_temp = str(min_temp).encode('utf-8')
    avg_temp = str(avg_temp).encode('utf-8')
    max_humidity = str(max_humidity).encode('utf-8')
    min_humidity = str(min_humidity).encode('utf-8')
    avg_humidity = str(avg_humidity).encode('utf-8')
    max_pressure = str(max_pressure).encode('utf-8')
    min_pressure = str(min_pressure).encode('utf-8')
    avg_pressure = str(avg_pressure).encode('utf-8')
    max_light = str(max_light).encode('utf-8')
    min_light = str(min_light).encode('utf-8')
    avg_light = str(avg_light).encode('utf-8')

    #HTTP PUT requests
    
    rp = requests.put(thingUrl+"/features/history/properties/max_temperature",
                     headers={'Authorization': authorization,
                              "Content-Type": "application/json",
                              "x-cr-api-token": apiToken}, data = max_temp);

    rp = requests.put(thingUrl+"/features/history/properties/min_temperature",
                     headers={'Authorization': authorization,
                              "Content-Type": "application/json",
                              "x-cr-api-token": apiToken}, data = min_temp);

    rp = requests.put(thingUrl+"/features/history/properties/avg_temperature",
                     headers={'Authorization': authorization,
                              "Content-Type": "application/json",
                              "x-cr-api-token": apiToken}, data = avg_temp);

    rp = requests.put(thingUrl+"/features/history/properties/max_humidity",
                     headers={'Authorization': authorization,
                              "Content-Type": "application/json",
                              "x-cr-api-token": apiToken}, data = max_humidity);

    rp = requests.put(thingUrl+"/features/history/properties/min_humidity",
                     headers={'Authorization': authorization,
                              "Content-Type": "application/json",
                              "x-cr-api-token": apiToken}, data = min_humidity);

    rp = requests.put(thingUrl+"/features/history/properties/avg_humidity",
                     headers={'Authorization': authorization,
                              "Content-Type": "application/json",
                              "x-cr-api-token": apiToken}, data = avg_humidity);

    rp = requests.put(thingUrl+"/features/history/properties/max_pressure",
                     headers={'Authorization': authorization,
                              "Content-Type": "application/json",
                              "x-cr-api-token": apiToken}, data = max_pressure);

    rp = requests.put(thingUrl+"/features/history/properties/min_pressure",
                     headers={'Authorization': authorization,
                              "Content-Type": "application/json",
                              "x-cr-api-token": apiToken}, data = min_pressure);

    rp = requests.put(thingUrl+"/features/history/properties/avg_pressure",
                     headers={'Authorization': authorization,
                              "Content-Type": "application/json",
                              "x-cr-api-token": apiToken}, data = avg_pressure);

    rp = requests.put(thingUrl+"/features/history/properties/max_light",
                     headers={'Authorization': authorization,
                              "Content-Type": "application/json",
                              "x-cr-api-token": apiToken}, data = max_light);

    rp = requests.put(thingUrl+"/features/history/properties/min_light",
                     headers={'Authorization': authorization,
                              "Content-Type": "application/json",
                              "x-cr-api-token": apiToken}, data = min_light);

    rp = requests.put(thingUrl+"/features/history/properties/avg_light",
                     headers={'Authorization': authorization,
                              "Content-Type": "application/json",
                              "x-cr-api-token": apiToken}, data = avg_light);

    print('PUT Request sent!')

    sleep(60)
    
# Close DB Connection
db.close()
