<h1>EECE 314 : Systems Software Engineering</h1>

<h3>Multi-threaded Elevator Simulation Demo</h3>

1. Download Raw copy <b>[ElevatorSimulator.zip](https://github.com/rmawong/EECE314/raw/master/ElevatorSimulation.zip)</b>  
  * There are two executable files: <b>ElevatorSystem.exe</b> and <b>ElevatorGraphics.exe</b>
  * <i>Important:</i> ensure that you keep both these files in the same folder
  
2. Run <b>ElevatorSystem.exe</b>

3. A console window will appear with `Enter number of elevators:`
  * You can enter any number between 1 and 9 inclusive
  * <i>Note:</i> avoid running more than 2 elevators if you do not have a quad core CPU

4. A second window will appear containing an elevator/floor animation

5. The following 2-character elevator commands can be entered:

| Floor Request Type | First Char             | Second Char            | Example      |
| -------------------|------------------------|------------------------|:------------:|
| Inside Request     | Elevator number [1..n] | Floor Number [0..9]    | `12` or `26` |
| Outside Request    | Direction {U, D}       | Floor Number [0..9]    | `U5` or `D9` |
| Halt               | {F}                    | Elevator number [1..n] | `F1` or `F5` |
| Clear              | {C}                    | Elevator number [1..n] | `C1` or `C5` |
| Exit Program       | {E}                    | {E}                    | `EE`         |
