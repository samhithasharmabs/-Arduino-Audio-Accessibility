BrailleBridge

A Modular, Repairable and Low-Cost Refreshable Braille Display

Vishwakarma Awards 2026–27 | Assistive Technology and Accessibility

1. Project Overview

BrailleBridge is a proposed refreshable Braille display designed to make digital information accessible through touch. The system aims to convert digital text into tactile Braille patterns using an electronically controlled mechanical pin mechanism.

The project focuses on developing an affordable, modular and repairable alternative to expensive refreshable Braille devices. Instead of relying on a fixed printed Braille document, the proposed system can update its tactile output as the displayed text changes.

The initial prototype focuses on one six-dot Braille cell. The design may later be expanded into a multi-cell display after the initial mechanism has been developed and validated.

2. Problem Statement

People who are blind or have low vision can face barriers when accessing digital information through touch. Phones and computers primarily present information visually, while printed or embossed Braille generally cannot be updated automatically when the source text changes.

Commercial refreshable Braille displays can also be expensive and mechanically complex, creating challenges around affordability, maintenance and repair.

BrailleBridge aims to address these challenges through a student-developed tactile display that converts digital text into refreshable Braille patterns using a modular mechanical design.

Key Challenges

* Limited access to changing digital text through tactile Braille.
* Static Braille documents cannot automatically update when digital content changes.
* The cost of refreshable Braille devices can be a barrier for some users.
* Complex mechanical assemblies can make maintenance and repair difficult.

3. Proposed Solution

BrailleBridge proposes an electronically controlled six-dot Braille cell in which selected tactile pins rise to represent a character while the remaining pins stay lowered.

The system is intended to receive text, convert characters into standard Grade 1 English Braille patterns and use actuators to generate the corresponding tactile output.

Proposed User Journey

1. Text Input: The user enters text through a supported input interface.
2. Braille Conversion: The firmware converts each supported character into a six-dot Braille pattern.
3. Electronic Control: The microcontroller sends control signals to the actuator-driving electronics.
4. Pin Movement: The mechanical mechanism raises or lowers the required Braille pins.
5. Tactile Reading: The user reads the displayed pattern by touch.
6. Text Navigation: Physical buttons are intended to support navigation through the entered text.

4. System Architecture

The proposed system consists of five main subsystems:

Input Subsystem

* Serial text input during the initial development stage.
* Physical buttons for text navigation.

Processing Subsystem

* Arduino Uno R3.
* Firmware for character handling and Braille pattern mapping.

Actuation Subsystem

* PCA9685 16-channel PWM servo driver boards.
* MG90S micro servos for the proposed pin-actuation mechanism.

Mechanical Subsystem

* Six-dot Braille cell.
* Tactile pins and mechanical linkages.
* A proposed 3D-printed pin guide and modular mechanical assembly.

Power Subsystem

* Proposed regulated 5 V power supply.
* Power distribution designed to support the controller, servo drivers and actuators.

The final mechanical dimensions, pin linkages, servo positions and operating parameters must be validated during prototype development.

5. Proposed Hardware Components

Component	Purpose
Arduino Uno R3	Main microcontroller
PCA9685 16-channel PWM servo drivers	Generate servo control signals
MG90S micro servos	Actuate the Braille pin mechanism
Regulated 5 V power supply	Supply the prototype electronics and servos
Push buttons	Provide navigation and control inputs
Braille pins and mechanical linkages	Produce the tactile dot patterns
3D-printed pin guide and mounting parts	Align and support the mechanical assembly
Connecting wires and mounting hardware	Electrical and mechanical integration

The current firmware design uses PCA9685 driver addresses 0x40 and 0x41. The initial six-dot implementation is intended to use six servo channels on the first driver. The second driver supports the proposed expansion architecture.

Component selection, power requirements, servo calibration and the final mechanical arrangement require verification during implementation.

6. Software and Firmware

The proposed firmware is written for the Arduino platform.

Planned functions include:

* Initialisation of the controller and servo drivers.
* Conversion of supported English letters into six-dot Braille patterns.
* Serial text input.
* Control of the six servo channels.
* Navigation through text using physical buttons.
* A startup sequence intended to reduce unwanted actuator movement.
* Servo position calibration to match the mechanical design.

The firmware is being developed alongside the mechanical prototype. Successful compilation, correct Braille mapping, reliable servo operation and complete button functionality must be verified before the system can be considered operational.

7. Design Priorities

Low Cost

Use accessible electronic components and a student-buildable mechanical assembly, with the total prototype cost documented through a bill of materials.

Modularity

Develop the display around a cell-based architecture that can potentially be expanded without redesigning the entire system.

Repairability

Design the pin guide, actuator mounting and supporting components so that individual parts can be inspected, removed and replaced where practical.

Accessibility

Generate tactile Braille patterns that users can read by touch, with simple navigation through text.

Expandability

Begin with one six-dot cell and investigate a multi-cell display only after the initial mechanism and control system have been validated.

8. Development Roadmap

Stage 1 — Design and Planning

* Define the problem and target beneficiaries.
* Select the proposed electronic architecture.
* Develop the six-dot cell concept.
* Identify the mechanical and electrical components.

Stage 2 — Initial Prototype

* Develop the six-dot mechanical assembly.
* Integrate the Arduino and servo driver.
* Implement and compile the initial firmware.
* Calibrate the actuator positions.

Stage 3 — Functional Testing

* Verify individual dot actuation.
* Test Braille patterns against a reference mapping.
* Evaluate text navigation.
* Record refresh behaviour, reliability and power performance.

Stage 4 — Improvement and Validation

* Improve pin alignment and mechanical reliability.
* Document prototype cost and repair procedures.
* Evaluate whether the initial design meets its intended performance targets.

Stage 5 — Future Expansion

* Investigate multiple Braille cells.
* Evaluate a replaceable cell cartridge and improved packaging.
* Assess additional input and navigation options.

9. Testing and Evaluation

The prototype will need to be evaluated against measurable engineering criteria.

Metric	Evaluation method
Braille pattern accuracy	Compare generated patterns against a standard Grade 1 English Braille reference
Dot actuation	Verify that each of the six dots can reach its intended raised and lowered positions
Refresh time	Measure the time required to update one character
Repeatability	Test repeated transitions between different Braille patterns
Navigation	Verify Previous, Next, Home and Reset behaviour
Power stability	Monitor the supply and controller behaviour during servo movement
Repairability	Demonstrate whether individual mechanical or electronic parts can be accessed and replaced
Prototype cost	Record component prices and calculate the total build cost

Test results, photographs and measured values will be added after the relevant tests have been performed. No performance result should be treated as verified until supported by actual testing.

10. Current Development Status

BrailleBridge is in the design and development stage.

The project architecture, proposed components and initial firmware structure have been documented. The next development priorities are to validate the firmware, finalise the mechanical pin-actuation design, assemble the initial six-dot cell and test the integrated system.

The prototype’s operational performance, Braille readability, reliability, refresh time and final cost remain to be established through implementation and testing.

11. Expected Impact

BrailleBridge aims to contribute to more affordable and maintainable access to digital information through tactile Braille.

The project explores whether a modular, student-developed mechanical display can provide a practical starting point for a refreshable Braille device while allowing future improvements to its cost, reliability and usability.

Its success will be evaluated through a working prototype, documented engineering decisions and measured test results rather than assumptions about performance.

12. Repository Structure

BrailleBridge/
├── README.md
├── Braillebridge.ino
├── hardware/
│   ├── bill-of-materials.md
│   └── wiring.md
├── mechanical-design/
│   └── design-notes.md
├── tests/
│   └── test-plan.md
└── images/
    └── README.md

Additional files will be added as the hardware, CAD models, documentation and test evidence become available.

13. Project Integrity

This repository documents the development of BrailleBridge. Proposed features, implemented features and experimentally verified results will be identified separately.

Photographs, CAD models, firmware revisions and test results will be added as the project progresses. Any future performance claims will be based on documented measurements.

⸻

BrailleBridge — Making digital information more accessible through tactile engineering.
