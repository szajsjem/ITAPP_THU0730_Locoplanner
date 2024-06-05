import pandas as pd
import random
from tqdm import tqdm
import get_pop
import json, os
from g4f.client import Client
stops = pd.read_csv(r'dataset/polish/stops.txt')
routes = pd.read_csv(r'dataset/polish/routes.csv')


df = pd.DataFrame(columns=['trip_id', 'start_station', 'end_station', 'start_time', 'end_time', 'start_lat', 'start_lon', 'end_lat',
                           'end_lon', 'start_city','start_city_pop', 'end_city','end_city_pop', 'country'])

cities = stops['stop_name'].unique()
cities = cities.tolist()
c2 = {}
if os.path.exists('dataset/polish/pop.json'):
    with open('dataset/polish/pop.json') as f:
        c2 = json.load(f)
    cities = [city for city in cities if city not in c2.keys()]
else:
    for city in tqdm(cities):
        c2[city] = get_pop.getpop(city)
        with open('dataset/polish/pop.json', 'w') as f:
            json.dump(c2, f)

client = Client()

for city in tqdm( c2):
    if c2[city] == 0:
        response = client.chat.completions.create(
            model="gpt-4-turbo",
            messages=[
                {"role": "system", "content": "Get the name of city the station named "+city+" is in. Answer only with the name of the city, as your answer is going to be piped into a function"},
            ]
        )
        print(response.choices[0].message.content)
        population = get_pop.getpop(response.choices[0].message.content)
        c2[city] = population
    

with open('dataset/polish/pop.json', 'w') as f:
    json.dump(c2, f)
    
stops = stops[['stop_id', 'stop_name']]
routes.drop(['stop_name_start', 'start_city_pop',"stop_name_end","end_city_pop"], axis=1, inplace=True)

df = routes.merge(stops, left_on='start_station', right_on='stop_id', how='left')
df = df.merge(stops, left_on='end_station', right_on='stop_id', how='left', suffixes=('_start', '_end'))
df['start_city_pop'] = df['stop_name_start'].map(c2)
df['end_city_pop'] = df['stop_name_end'].map(c2)

df['country'] = 'Poland'
df = df[['trip_id', 'start_station', 'end_station', 'start_time', 'end_time', 'start_lat', 'start_lon', 'end_lat',
         'end_lon', 'stop_name_start', 'start_city_pop', 'stop_name_end', 'end_city_pop', 'country']]
df.to_csv(r'dataset/polish/routes2.csv', index=False)
