import serial, csv


ser = serial.Serial('/dev/ttyACM0', 9600)
reading_list = []

while 1:
    if(ser.in_waiting >0):
        line = ser.readline()
        try:
            string = line.decode('utf-8')
        except:
            string = "-,-,-,-"
            pass

        string.replace('\r\n', '').replace('\n', '')

        string = string.split(',')





        with open('output.csv', 'a') as outputFile:
            writer = csv.writer(outputFile)
            writer.writerow(string)


