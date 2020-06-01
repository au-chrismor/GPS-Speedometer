"""
Simple tool to write data out as a gpx file
Allows import into Strava and other applications
"""
import argparse
import csv
from xml.etree.ElementTree import ElementTree, Element, SubElement

ap = argparse.ArgumentParser()
ap.add_argument('-i', '--input', type=str, default = 'TEST.CSV', help = 'Name of input file')
ap.add_argument('-o', '--output', type=str, default = 'ride.gpx', help = 'Name of output file')
args = ap.parse_args()
root = Element('?xml')
root.set('version', '1.0')
gpx = SubElement(root, 'gpx')
gpx.set('version', '1.1')
gpx.set('creator', 'GPS-Speedometer')
gpx.set('xmlns', 'http://www.topografix.com/GPX/1/1')
gpx.set('xmlns:xsi', 'http://www.w3.org/2001/XMLSchema-instance')
gpx.set('xsi:schemaLocation', 'http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd')
gpx.set('xmlns:gpxtpx', 'http://www.garmin.com/xmlschemas/TrackPointExtension/v1')
with open(args.input, 'r') as csvFile:
    csvReader = csv.reader(csvFile)
    rowNum = 0
    for row in csvReader:
        wpt = SubElement(root, 'wpt')
        wpt.set('lat', row[7])
        wpt.set('lon', row[8])
        ele = SubElement(wpt, 'ele')
        ele.text = row[9]
        tme = SubElement(wpt, 'time')
        tme.text = '{:04d}-{:02d}-{:02d}T{:02d}:{:02d}:{:02d}Z'.format(int(row[1]), int(row[2]), int(row[3]), int(row[4]), int(row[5]), int(row[6]))
csvFile.close()
ElementTree(root).write(str(args.output), method='xml')
