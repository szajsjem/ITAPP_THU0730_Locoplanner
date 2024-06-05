import requests

# Set the API endpoint
API_ENDPOINT = "https://www.wikidata.org/w/api.php"

# Replace 'SEARCH_TERM' with the name you're searching for
def getpop(search_term):
    # Define the parameters for the API request
    params = {
        'action': 'wbsearchentities',
        'format': 'json',
        'language': 'en',
        'search': search_term
    }

    # Send the GET request
    response = requests.get(API_ENDPOINT, params=params)

    # Check if the request was successful
    if response.status_code == 200:
        # Parse the JSON response
        data = response.json()
        
        # Access the search results
        search_results = data.get('search', [])
        
        # If there are results, print the first one
        for entity in search_results:
            # Get the ID of the first result
            entity_id = entity['id']
            
            # Fetch the document for the entity using its ID
            document_params = {
                'action': 'wbgetentities',
                'ids': entity_id,
                'format': 'json'
            }
            
            # Send the GET request for the document
            document_response = requests.get(API_ENDPOINT, params=document_params)
            
            if document_response.status_code == 200:
                # Print the document
                document_data = document_response.json()
                if document_data["entities"].get(entity_id).get('claims').get('P1082'):
                    return int(document_data["entities"].get(entity_id).get('claims').get('P1082')[0].get('mainsnak').get('datavalue').get('value').get('amount').strip("+"))
                else:
                    continue
            else:
                continue
    
        return 0
    else:
        return 0


print(getpop('Warsaw'))

def City_finder(name):
    # Define the parameters for the API request
    params = {
        'action': 'wbsearchentities',
        'format': 'json',
        'language': 'en',
        'search': name,
        'type': 'item',
        'limit': 1,
        'props': 'claims'
    }
    
    # Send the GET request
    response = requests.get(API_ENDPOINT, params=params)
    
    # Check if the request was successful
    if response.status_code == 200:
        # Parse the JSON response
        data = response.json()
        
        # Access the search results
        search_results = data.get('search', [])
        
        # If there are results, print the first one
        if search_results:
            # Get the ID of the first result
            entity_id = search_results[0]['id']
            
            # Fetch the document for the entity using its ID
            document_params = {
                'action': 'wbgetentities',
                'ids': entity_id,
                'format': 'json'
            }
            
            # Send the GET request for the document
            document_response = requests.get(API_ENDPOINT, params=document_params)
            
            if document_response.status_code == 200:
                # Print the document
                document_data = document_response.json()
                if document_data["entities"].get(entity_id).get('claims').get('P131'):
                    city_id = document_data["entities"].get(entity_id).get('claims').get('P131')[0].get('mainsnak').get('datavalue').get('value').get('id')
                    city_name = get_city_name(city_id)
                    return city_name
                else:
                    return "City not found"
            else:
                return "City not found"
        else:
            return "City not found"
    else:
        return "City not found"

def get_city_name(city_id):
    # Define the parameters for the API request
    params = {
        'action': 'wbgetentities',
        'ids': city_id,
        'format': 'json'
    }
    
    # Send the GET request
    response = requests.get(API_ENDPOINT, params=params)
    
    # Check if the request was successful
    if response.status_code == 200:
        # Parse the JSON response
        data = response.json()
        
        # Get the city name
        city_name = data["entities"].get(city_id).get('labels').get('en').get('value')
        return city_name
    else:
        return "City not found"

print(City_finder('Warszawa Zachodnia'))