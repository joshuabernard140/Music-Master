import sys
import os
from pytube import YouTube

# Check if the URL is provided as a command-line argument
if len(sys.argv) < 2:
    print("Usage: python audioDownload.py <URL>")
    sys.exit(1)

url = sys.argv[1]  # Get the URL from the command-line argument
target_directory = "downloads"

try:
   video = YouTube(url)
   stream = video.streams.filter(only_audio=True).first()

   if not os.path.exists(target_directory):
       os.makedirs(target_directory)

   file_path = os.path.join(target_directory, f"{video.title}.mp3")
   stream.download(filename=file_path)
   print("The video is downloaded in MP3")
except Exception as e:
   print(f"An error occurred: {e}")                                                                                          