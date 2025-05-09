```plantuml
' Define the styles for the Context diagram.

' Define the system context.
' Add the terminals
Rectangle "idle" as Terminal1 
Rectangle "line tracking" as Terminal2 
Rectangle "blocked" as Terminal3 
Rectangle "finished" as Terminal4 
Rectangle "ramp" as Terminal5

' Add the relationships between the terminals and the system.
Terminal1 --> Terminal2 :switched on
Terminal2 --> Terminal3 :distance less than x cm
Terminal3 --> Terminal2 :distance more than x cm
Terminal2 --> Terminal4 :line detected on all sensors
Terminal2 --> Terminal1 :switched off
Terminal3 --> Terminal1 :switched off
Terminal4 --> Terminal1 :switched off
Terminal2 --> Terminal5 :tilt enabled
Terminal5 --> Terminal2 :tilt disabled
Terminal5 --> Terminal1 :switched off