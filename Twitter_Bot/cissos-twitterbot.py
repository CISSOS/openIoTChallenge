import requests
import tweepy
import time
from time import sleep

def tweet():

        while 1:

                r = requests.get("https://Url-to-Ditto.com/api/2/things/come.your.namespace:YOURTHING/", #Use ur own URL to your Twin
                                headers={'Authorization': 'Basic xxxxxxxxxxxxxxxxxxxxx', #Use your base64 encrypted username:password
                                "Content-Type": "application/json",
                                "x-cr-api-token": "xxxxxxxxxxxxxxxxx"});#Your API Token

                data = r.json()

                CONSUMER_KEY = '1234abcd...'#keep the quotes, replace this with your consumer key
				CONSUMER_SECRET = '1234abcd...'#keep the quotes, replace this with your consumer secret key
				ACCESS_KEY = '1234abcd...'#keep the quotes, replace this with your access token
				ACCESS_SECRET = '1234abcd...'#keep the quotes, replace this with your access token secret


                auth = tweepy.OAuthHandler(CONSUMER_KEY, CONSUMER_SECRET)
                auth.set_access_token(ACCESS_TOKEN, ACCESS_SECRET)

                api = tweepy.API(auth)

                temp = data['features']['temperature']['properties']['value'];
                maxtemp = data['features']['history']['properties']['max_temperature'];
                mintemp = data['features']['history']['properties']['min_temperature'];
                avgtemp = data['features']['history']['properties']['avg_temperature'];

                light = data['features']['light']['properties']['value'];
                maxlight = data['features']['history']['properties']['max_light'];
                minlight = data['features']['history']['properties']['min_light'];
                avglight = data['features']['history']['properties']['avg_light'];

                hum = data['features']['humidity']['properties']['value'];
                maxhum = data['features']['history']['properties']['max_humidity'];
                minhum = data['features']['history']['properties']['min_humidity'];
                avghum = data['features']['history']['properties']['avg_humidity'];

                pres = data['features']['pressure']['properties']['value'];
                maxpres = data['features']['history']['properties']['max_pressure'];
                minpres = data['features']['history']['properties']['min_pressure'];
                avgpres = data['features']['history']['properties']['avg_pressure'];

                temp = int(temp)
                temp = temp/1000
                maxtemp = int(maxtemp)
                maxtemp = maxtemp/1000
                mintemp = int(mintemp)
                mintemp = mintemp/1000
                avgtemp = int(avgtemp)
                avgtemp = avgtemp/1000

                light = int(light)
                light = light/10
                maxligt = int(maxlight)
                maxlight = maxlight/10
                minlight = int(minlight)
                minlight = minlight/10
                avglight = int(avglight)
                avglight = avglight/10

                hum = int(hum) 
                maxhum = int (maxhum)
                minhum = int(minhum)
                avghum = int(avghum)

                pres = int(pres)
                pres = pres/100
                maxpres = int(maxpres)
                maxpres = maxpres/100
                minpres = int(minpres)
                minpres = minpres/100
                avgpres = int(avgpres)
                avgpres = avgpres/100

                api.update_status("Datum und Uhrzeit: "+time.strftime("%d.%m.%Y %H:%M:%S")+", "
                          "Aktuelle Temperatur: "+str(temp)+" °C,  "
                          "Maximale Temperatur: "+str(maxtemp)+" °C, "
                          "Minimale Temperatur: "+str(mintemp)+" °C, "
                          "Durchschnittliche Temperatur: "+str(avgtemp)+" °C"
                          )
                api.update_status("Datum und Uhrzeit: "+time.strftime("%d.%m.%Y %H:%M:%S")+", "
                          "Aktuelle Beleuchtungsstärke: "+str(light)+" lux, "
                          "Maximale Beleuchtungsstärke: "+str(maxlight)+" lux, "
                          "Minimale Beleuchtungsstärke: "+str(minlight)+" lux, "
                          "Durchschnittliche Beleuchtungsstärke: "+str(avglight)+" lux"
                          )
                api.update_status("Datum und Uhrzeit: "+time.strftime("%d.%m.%Y %H:%M:%S")+", "
                          "Aktuelle Luftfeuchtigkeit: "+str(hum)+" %, "
                          "Maximale Luftfeuchtigkeit: "+str(maxhum)+" %, "
	                  "Minimale Luftfeuchtigkeit: "+str(minhum)+" %, "
                          "Durchschnittliche Luftfeuchtigkeit: "+str(avghum)+" %"
                          )
                api.update_status("Datum und Uhrzeit: "+time.strftime("%d.%m.%Y %H:%M:%S")+", "
                          "Aktueller Luftdruck: "+str(pres)+" hPa, "
                          "Maximaler Luftdruck: "+str(maxpres)+" hPa, "
                          "Minimaler Luftdruck: "+str(minpres)+" hPa, "
                          "Durchschnittlicher Luftdruck: "+str(avgpres)+" hPa"
                          )
                sleep(120)
               
tweet()	

	
