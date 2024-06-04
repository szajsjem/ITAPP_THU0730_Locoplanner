import pandas as pd
from phi.assistant import Assistant
from phi.tools.duckduckgo import DuckDuckGo
from phi.tools.wikipedia import WikipediaTools
from phi.llm.ollama import Ollama
import re
import json
import pydantic
import ollama
import wikipedia
from phi.tools import Toolkit

class Response(pydantic.BaseModel):
    population: int


stops = pd.read_csv(r'dataset/polish/stops.txt')


        
class myTool(Toolkit):
    def __init__(self):
        super().__init__(name="wikipedia_tool")
        self.register(self.population_tool)

    def population_tool(self, city, country):
        """Finds the information about a city in a country using wikipedia.

        Args:
            city (string): city name
            country (string): country name

        Returns:
            string: data about the population of the city
        """
        target_page = wikipedia.search("population of " + city + ", " + country)
        if target_page:
            page = wikipedia.page(target_page[0])
            summary = ollama.chat(model="phi3", messages=[
                {"role": "system", "content": "Get the population of " + city + ", " + country + ". You will be given the wikipedia page as a source. \
                Use only data from the source, do not make up a number. If you cannot find the number, say it clearly."},
                {"role": "user", "content": page.content + " " + " ".join(page.sections)}
            ])
            return summary["message"]["content"]
        
        
assistant = Assistant(
    llm = Ollama(model="phi3"),
    tools=[DuckDuckGo(), myTool()],
    description="You are a web search agent that answers in short sentences.",
    instructions=["Find the number of people in the city the name you will be given.", "Answer only with the number, nothing else.", "Use wikipedia as primary source", ],
    show_tool_calls=True,
    output_model=Response
    
)
results = []
for stop in stops.iterrows():
    stop = stop[1]
    city = stop['stop_name']
    country = 'Poland'
    done = False
    while not done:
        possible_number = assistant.run("How many people live in " + city + ", " + country + "?", stream=False)
        print(possible_number)
        if re.findall(r'\d[d,\,]+', possible_number):
            number = re.findall(r'\d[d,\,]+', possible_number)[0]
            done = True
    results.append([stop, city, country, number])
    open('dataset/polish/stations.txt', 'w').writelines(",".join(results))


