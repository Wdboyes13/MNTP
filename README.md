# Metric Network Time Protocol
Inspired by [metric-time.com](https://metric-time.com/)  
This is a little thing, designed to make "Metric Time" easily accesible  

## The protocol
The MNTP Server, can serve ALL timezones   
And the data is done over TCP on port 6980  
More info about the protocol structure is at [PacketStructure.txt](/PacketStructure.txt)

## The Client
The MNTP Client, will soon be upgraded. 
Currently you enter a timezone (Like Canada/Pacific or America/Los_Angeles)  
Then you enter the Server IP.  
After that, you will be asked if you want to use the default port, it is 6980.  
If you're server is not on that port, hit N or n, if it is, hit anything else.  
If you selected ton use a non-default port, you will then be asked what port to use.  
After this, it will return the time in \<Metric Hours\>-\<Metric Minutes\>-<Metric Seconds\>  
  
A server you can use, is on 147.185.221.30  And uses the non-default port of 36483  