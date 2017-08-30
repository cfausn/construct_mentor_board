from __future__ import print_function
import httplib2
import os
import csv

from apiclient import discovery
from oauth2client import client
from oauth2client import tools
from oauth2client.file import Storage
from dateutil import parser

import datetime
import time
import threading

try:
    import argparse
    flags = argparse.ArgumentParser(parents=[tools.argparser]).parse_args()
except ImportError:
    flags = None

# If modifying these scopes, delete your previously saved credentials
# at ~/.credentials/calendar-python-quickstart.json
SCOPES = 'https://www.googleapis.com/auth/calendar.readonly'
CLIENT_SECRET_FILE = 'client_secret.json'
APPLICATION_NAME = 'Google Calendar API Python Quickstart'


def get_credentials():
    """Gets valid user credentials from storage.

    If nothing has been stored, or if the stored credentials are invalid,
    the OAuth2 flow is completed to obtain the new credentials.

    Returns:
        Credentials, the obtained credential.
    """
    home_dir = os.path.expanduser('~')
    credential_dir = os.path.join(home_dir, '.credentials')
    if not os.path.exists(credential_dir):
        os.makedirs(credential_dir)
    credential_path = os.path.join(credential_dir,
                                   'calendar-python-quickstart.json')

    store = Storage(credential_path)
    credentials = store.get()
    if not credentials or credentials.invalid:
        flow = client.flow_from_clientsecrets(CLIENT_SECRET_FILE, SCOPES)
        flow.user_agent = APPLICATION_NAME
        if flags:
            credentials = tools.run_flow(flow, store, flags)
        else: # Needed only for compatibility with Python 2.6
            credentials = tools.run(flow, store)
        print('Storing credentials to ' + credential_path)
    return credentials

def main():
    """Shows basic usage of the Google Calendar API.

    Creates a Google Calendar API service object and outputs a list of the next
    10 events on the user's calendar.
    """
    credentials = get_credentials()
    http = credentials.authorize(httplib2.Http())
    service = discovery.build('calendar', 'v3', http=http)

    now = datetime.datetime.utcnow().isoformat() + 'Z' # 'Z' indicates UTC time
    eventsResult = service.events().list(
        calendarId='jm3ncjbomu6jasgq2nm3ni5lu0@group.calendar.google.com', timeMin=now, maxResults=2, singleEvents=True,
        orderBy='startTime').execute()
    events = eventsResult.get('items', [])



    if not events: print('No upcoming events found.')
    else: threading.Thread(target=getStatuses, args=(events,)).start()
    
    # TODO: Add Serial Port communication to update the "status" variable within an event.
    #       Add a "isconnected" variable as well to help out the webpage w/ knowing if 
    #       the BT connection is active.


def getStatuses(events):
    
    with open('html/data/schedules.csv','w') as csvfile:
       fieldnames = ['name', 'start', 'end','isWorking','status']
       writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
       writer.writeheader()

       for event in events:
           start = event['start'].get('dateTime', event['start'].get('date'))
           end = event['end'].get('dateTime', event['end'].get('date'))
           present = datetime.datetime.now()
           endString = end[:-6]
           startString = start[:-6]
           isWorking = False
           status = ''
           startDateTime = datetime.datetime.strptime(startString, "%Y-%m-%dT%H:%M:%S")
           endDateTime = datetime.datetime.strptime(endString, "%Y-%m-%dT%H:%M:%S")
           if present < endDateTime and present > startDateTime:
              isWorking = True

 
           writer.writerow({'name': event['summary'], 'start':start, 'end':end, 'isWorking':isWorking, 'status':status})


           #print(start,end, event['summary'])
    print("Updated CSV")
    time.sleep(5)
    getStatuses(events)

if __name__ == '__main__':
    main()
