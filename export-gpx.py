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
gpx = Element('gpx')
gpx.set('version', '1.1')
gpx.set('creator', 'GPS-Speedometer')
gpx.set('xmlns', 'http://www.topografix.com/GPX/1/1')
gpx.set('xmlns:xsi', 'http://www.w3.org/2001/XMLSchema-instance')
gpx.set('xsi:schemaLocation', 'http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd')
gpx.set('xmlns:gpxtpx', 'http://www.garmin.com/xmlschemas/TrackPointExtension/v1')
trk = SubElement(gpx, 'trk')
trkName = SubElement(trk, 'name')
trkName.text = 'Simple GPS Log'
trkSeg = SubElement(trk, 'trkseg')
with open(args.input, 'r') as csvFile:
    csvReader = csv.reader(csvFile)
    rowNum = 0
    for row in csvReader:
        trkPt = SubElement(trkSeg, 'trkpt')
        trkPt.set('lat', row[7])
        trkPt.set('lon', row[8])
        ele = SubElement(trkPt, 'ele')
        ele.text = row[9]
        tme = SubElement(trkPt, 'time')
        tme.text = '{:04d}-{:02d}-{:02d}T{:02d}:{:02d}:{:02d}Z'.format(int(row[1]), int(row[2]), int(row[3]), int(row[4]), int(row[5]), int(row[6]))
csvFile.close()
ElementTree(gpx).write(str(args.output), method='xml', xml_declaration=True, encoding='UTF-8')
