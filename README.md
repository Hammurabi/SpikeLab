
# SpikeLab
A set of tools for neural-simulation, debugging, and visualization.

## What SpikeLab isn't
SpikeLab is not an accurate neural-simulator, the reason for this comes from the complexity of the biological neuron, there are too many mechanisms that give rise to the behavior of a neuron, simulating all of these mechanisms for a large scale network of neurons would be too resource intensive, therefore SpikeLab simulates the excitation/inhibition and temporal summation mechanisms that give rise to an action potential with minimal branching so we could simulate hundreds or thousands of neurons on retail hardware without issues.

## What SpikeLab Is
SpikeLab is a sandbox neural-simulation framework/engine, you can create complex neural-circuitry and debug them in real-time.
With SpikeLab you can generate different types of Neurons and allow them to communicate with each-other in new and interesting ways, you can change their behavior and functionality, design your own circuits or let the Neurons find their own way of communication for emergent behavior.

#### Can The Network Learn Something On Its Own?
Of course! The plan is to even add image, video, and audio file support so you could tune your network to take specific types of inputs from real world data! Scroll down to the "Goals" section to find out more!



But enough text, here are some gifs for you to look at!


### Simulate Action Potentials With The Click Of A Button!
![](https://raw.githubusercontent.com/Hammurabi/SpikeLab/master/gifs/action%20potentials.gif)

With SpikeLab you can simulate neural-excitation/inhibition and generate action potentials.

### Create Timed Neural-Circuits!
![enter image description here](https://raw.githubusercontent.com/Hammurabi/SpikeLab/master/gifs/timed%20circuits.gif)

You can also create chain neurons together to create timed circuits that repeat specific inputs!
### The Sky's The Limit!
Complex circuits can  be created with SpikeLab that can give rise to new patterns and unexpected behavior, so have at it!

## I Want To Learn To Make My Own Simulation
So you want to create your own simulator? That's great! Scroll down to the "References" tab for the resources I used to create this project.

## Features

 - [x] Neuron simulator
 - [x] Real-time line chart to view action potentials
 - [x] Simple documented framework

## Goals
SpikeLab is still in its infancy and is still a barebones framework, here is a list of features we are hoping to implement in the future.

 1. Image support (JPEG, PNG, BMP)
 2. Video support (MP4, GIF)
 3. Audio support (WAV, MP3)
 4. Scripting support (LUA or Python)
 5. GPGPU support (OpenCL)
 6. Full editing toolkit (realtime in-engine)

## Contributing
If you are interested in the subject and would like to contribute to this project, feel free to join our public [Discord server](https://discord.gg/hudEw3e) or add on Discord directly (Sensei#3409)

 

## References
[The fundamentals of Neuroscience](https://www.edx.org/xseries/harvardx-fundamentals-of-neuroscience) on edX is a great first start, Dr. David Cox created a series of three awesome courses that are free and I recommend it to everyone interested in the subject. I also recommend getting certified on edX as you can get a cool [certificate](https://courses.edx.org/certificates/13361343b546445db3221f6a25862912) to show off your effort :)
