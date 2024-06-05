import requests

def query_wikidata(sparql_query):
    url = 'https://query.wikidata.org/sparql'
    headers = {'Accept': 'application/sparql-results+json'}
    response = requests.get(url, headers=headers, params={'query': sparql_query})
    response.raise_for_status()
    return response.json()

def get_station_city(station_name):
    sparql_query = f"""
    SELECT ?city ?cityLabel WHERE {{
      ?station (rdfs:label|skos:altLabel) "{station_name}"@en.
      ?station wdt:P31/wdt:P279* wd:Q55488.
      ?station wdt:P131 ?city.
      SERVICE wikibase:label {{ bd:serviceParam wikibase:language "en". }}
    }}
    LIMIT 1
    """
    result = query_wikidata(sparql_query)
    bindings = result['results']['bindings']
    if bindings:
        city = bindings[0]['city']['value']
        city_label = bindings[0]['cityLabel']['value']
        return city, city_label
    else:
        print(f"No city found for station: {station_name}")
    return None, None

def get_city_population(city_id):
    sparql_query = f"""
    SELECT ?population WHERE {{
      wd:{city_id} wdt:P1082 ?population.
    }}
    LIMIT 1
    """
    result = query_wikidata(sparql_query)
    bindings = result['results']['bindings']
    if bindings:
        return int(bindings[0]['population']['value'])
    else:
        print(f"No population data found for city ID: {city_id}")
    return None

def get_population_by_station(station_name):
    city_id, city_label = get_station_city(station_name)
    if city_id:
        population = get_city_population(city_id.split('/')[-1])
        if population:
            return population
        else:
            print(f"Population data not found for city: {city_label}")
    else:
        print(f"City not found for station: {station_name}")
    return None
# Example usage
station_name = "Warszawa Zachodnia"
print(get_station_city(station_name))
population = get_population_by_station(station_name)
print(f"The population of the city associated with {station_name} is {population}")
