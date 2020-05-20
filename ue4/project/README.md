# Vivado Project

This project generates a simple application for the Avnet MINIZED board. \
Features include:

- I2C Sensor
- LED control (PS LED and PL LED)

## Open the Vivado project

1. Generate the Vivado project. \
   Execute the script `./scripts/run.sh project`.
2. Open the Vivado project. \
   `vivado BasicSystem.xpr`

## Run this project on the MICROZED Board

1. Generate and open the SDK project. \
   `./scripts/run.sh sdk`
2. Close the "Welcome" window. The default C/C++ view should appear.
3. In the Project-Explorer right-click the *app* and click `Build Project`. The project should successfully compile.
4. Click `Xilinx --> Program FPGA`.
5. In the Project-Explorer right-click the *app* and click `Debug As --> 1 Launch on Hardware (System Debugger)`.
6. The debug perspective opens and the firmware should run into an automatic breakpoint at the beginning of `main()`.
7. Click the `Resume` button in the toolbar.
8. To stop debugging, make sure to click the `disconnect` button, before doing anything else. \
Failure to do so, will bring Eclipse in a weird state and it will never work again, unless you re-generate the whole project (delete generated output files, then go to step 1). Don't ask why.. we will never know.... It's Eclipse..

## Update the firmware through the SDK

1. Get the latest bitstream (\*.bit) and Hardware Definition File (\*.hdf) from the Vivado build output.
2. Replace the respective files in the sdk folder.
   - sdk/basic_design_wrapper_hw_platform_0/basic_design_wrapper.bit
   - sdk/basic_design_wrapper.hdf
3. Delete the folder `RemoteSystemsTempFiles` if it exists.
4. Open the SDK project. \
   `./scripts/run.sh sdk` \
   This will automatically re-generate the BSP.
5. Continue from step 3 [in the step above](#run-this-project-on-the-microzed-board).

## Troubleshooting

This section describes common errors that may occur during development.

### Cannot re-generate BSP

**Situation:**

The sdk/basic_design_wrapper_hw_platform_0/system.hdf was replaced with a new version, which requires a firmware change. A click on `Regenerate BSP` does not automatically include new sources, such as header files.

**Solution:**

1. Delete the BSP (make sure to remember its name).
2. Click `New --> Board Support Package` to generate a new BSP with the old name.
3. The BSP should now include the new sources, to support IP blocks that are instantiated in the new HDF and bitstream.

### Program FPGA failed

**Situation:**

The exact error message is: \
_Program FPGA failed \
   Reason: \
   fpga configuration failed. DONE PIN is not HIGH_

Above error message occurs, after flashing the FPGA.

**Solution:**

1. Reset FPGA board (Power cycle)
2. Flash it again (Click `Xilinx --> Program FPGA` in SDK project)

### Error while launching program. Memory write error at 0x0.

**Situation:**

The exact error message is: \
_Error while launching program: \
Memory write error at 0x0. Cannot read sctlr_el3. Cannot read r0. Instruction transfer timeout_

Above error message occurs, when starting a debug session. \
The error does not recover by restarting the debug session.

**Solution:**

1. Reset FPGA board (Power cycle)
2. Flash it again (Click `Xilinx --> Program FPGA` in SDK project)
3. Restart debug session (`Debug As --> 1 Launch on Hardware (System Debugger)`).

In case the error persists, repeat these 3 steps.

It may rarely occur that above steps won't help and the error still persists. \
As a last measure, delete several temporary project files and [re-generate the project](#run-this-project-on-the-microzed-board). \
