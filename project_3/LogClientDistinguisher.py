import sys
from typing import List


class LogClientDistinguisher:
    def __init__(self, desktop_file_name, mobile_file_name):
        self.desktop = self.read_file(desktop_file_name)
        self.mobile = self.read_file(mobile_file_name)
        self.process_log()

    @staticmethod
    def read_file(file_name: str) -> List[str]:
        with open(file_name, 'r') as file:
            result = []
            for line in file:
                result.append(line.strip())
            return result

    def get_line_client(self, line_client: str) -> str:
        result = 'unknown'

        for known_agent in self.desktop:
            if known_agent == line_client:
                result = 'desktop'

        for known_agent in self.mobile:
            if known_agent == line_client:
                result = 'mobile'

        return result

    @staticmethod
    def get_line_agent(line: str) -> List[str]:
        # returns agent substring from log line

        first_substring = line.find('"')
        end_first_substring = line.find('"', first_substring + 1)

        second_substring = line.find('"', end_first_substring + 1)
        end_second_substring= line.find('"', second_substring + 1)

        agent_substring = line[second_substring+1:end_second_substring]
#        print(agent_substring)
        return(agent_substring)

    def process_log(self):
        with open('stdin.txt', 'r') as file:
            for line in file:
              agent = self.get_line_agent(line)
              client = self.get_line_client(agent)
              print(client + ' ' + line.strip())


if __name__ == '__main__':
    LogClientDistinguisher('d.txt', 'm.txt')
