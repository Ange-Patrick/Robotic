# Robotic
Codes used during the national robotics and AI olympics organized by ELVIATECH in CAMEROON 2023. 

Since 2022,the national robotics and AI olympics have been taking place in Cameroon. the aim is to solve a few challenges in teams of 2 or 3 members.
In this file, I will describe :
1- how the competition unfolded;
2- the organization of this repository.

Before I start, I'd like to thank my partners Gabin NDJOUTSE and Kaïs MOMO. The success would'nt have been possible without their motivation... Of course we (FABULOUS TRIBUS Team) won😂⭐.

Now, let's go...😉.

**1- How the competiton runs ?**
The competition pits differents teams coming from the country's leading schools and professionals (companies). Each team is made up of two or three members. 

Some materials (car chassis, wheels, motors and screws) are supplied by the organizers but others (multimeter, soldering iron, battery, etc) must be brought along by the team.

Pre-programmed and pre-assembled robots are not permitted.

The preparation time is approximatively 2 weeks : the delay between the competition day and the day on which the documents and materials are supplied. The documents describe each challenge in detail.

The aim is to get the maximum number of points by adding the results obtained for each challenge. the competition consists of 8 challenges, each with his own number of points. These challenges are : 
   - ROBOTS RACE : the race is carried out by groups of two robots, each of them driving on a track. The robot can be controlled through a remote control.
   - ROBOT MAZE  : for this event, the robot will have to cross a maze. The robot are AUTONOMOUS.
   - ROBOT LINE FOLLOWER : the robots have to follow a black line. The robot are AUTONOMOUS.
   - ROBOTS SUMO : the classic SUMO challenge. The robot are AUTONOMOUS.
   - ROBOTS FIGHT : two teams face off in an arena. The robot can be controlled.
   - ROBOT AQUATIC : Race in the Water 😪(our robots had sunk). The robot can be controlled.
   - INGENUITY CHALLENGE : AUTONOMOUS vehicle which evolve in a mini agricultural environment in order to harvest fruit and store them in a container that it carries. 
   - INNOVATION CONTEST :  Present a project related to the theme of competition : robotics and new technologies for the management and automation of healthy and susbstainable agriculture.


**2- How is organized this repository**
  
  Repository : Robotic
  
   - file : README.md --> the present file
   - file : challenges_description.pdf --> Gives detailed information (rules, number of points, etc) about the different expectations for each challenge.
   - folder : MAZE
     
               * file : maze.png : Image showing the materials configuration for the maze challenge.             
               * file : maze_esp_wall_follower_vf.INO --> code running on the microcontroller (ESP32 in our case).
               * file : result.MP4 --> an excerpt from the video showing how the robot works in the MAZE.
     
   - folder : LINE_FOLLOWER
     
               * file : line_follower.png : Image showing the materials configuration for the line follower challenge.             
               * file : suiveur_ligne_esp_vf.INO --> code running on the microcontroller (ESP32 in our case).
               * file : result.MP4 --> an excerpt from the video showing how the robot works in the MAZE.

   - folder : SUMO
     
               * file : SUMO.png : Image showing the materials configuration for the SUMO challenge.             
               * file : SUMO_vf.INO --> code running on the microcontroller (ESP32 in our case).

  
  
