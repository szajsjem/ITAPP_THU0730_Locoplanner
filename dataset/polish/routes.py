import pandas as pd
import gtfs_kit as gk
import tqdm
routes = pd.read_csv(r'dataset/polish/routes.txt')
stops = pd.read_csv(r'dataset/polish/stops.txt')
trips = pd.read_csv(r'dataset/polish/trips.txt')
stop_times = pd.read_csv(r'dataset/polish/stop_times.txt')

target = []

trip_ids = trips['trip_id'].unique()

grouped = stop_times.groupby('trip_id')

for group in tqdm.tqdm(grouped):
    trip= (group[0],[])
    start_station = group[1].iloc[0]['stop_id']
    start_time = group[1].iloc[0]['departure_time']
    for id,line in group[1].iterrows():
        end_station = line['stop_id']
        end_time = line['arrival_time']
        trip[1].append([start_station, end_station,start_time, end_time])
        start_station = end_station
        start_time = line['departure_time']
    target.append(trip)

# Create an empty dataframe to store the transformed data
df = pd.DataFrame(columns=['trip_id', 'start_station', 'end_station', 'start_time', 'end_time', 'start_lat', 'start_lon', 'end_lat', 'end_lon', 'city', 'country'])

# Iterate over each trip in the target list
for trip in tqdm.tqdm(target):
    trip_id = trip[0]
    trip_data = trip[1]
    
    # Iterate over each segment in the trip data
    for segment in trip_data:
        start_station = segment[0]
        end_station = segment[1]
        start_time = segment[2]
        end_time = segment[3]
        
        # Get the coordinates for the start and end stations from the stops dataframe
        start_coords = stops.loc[stops['stop_id'] == start_station, ['stop_lat', 'stop_lon']].values[0]
        end_coords = stops.loc[stops['stop_id'] == end_station, ['stop_lat', 'stop_lon']].values[0]
        
        # Append the data to the dataframe
        new_row = pd.DataFrame({'trip_id': [trip_id], 'start_station': [start_station], 'end_station': [end_station],
                                'start_time': [start_time], 'end_time': [end_time], 'start_lat': [start_coords[0]], 
                                'start_lon': [start_coords[1]], 'end_lat': [end_coords[0]], 'end_lon': [end_coords[1]]})
        df = pd.concat([df, new_row], ignore_index=True)
        # Print the resulting dataframe

df.to_csv(r'dataset/polish/routes.csv', index=False)


