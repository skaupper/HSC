/*****************************************************************************/
/**
 * @file    emu_cpu.cpp
 * @author  Michael Wurm <michael.wurm@students.fh-hagenberg.at>
 *          Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
 * @brief   EmuCPU implementation
 */
/*****************************************************************************/

#include "emu_cpu.hpp"

EmuCpu::EmuCpu(sc_module_name module_name, main_func_ptr_t main_entry_point)
    : sc_module(name), mMainFuncPtr(main_entry_point)
{
  //SC_THREAD(stimuli_process);
}

EmuCpu *EmuCpu::getInstance()
{
  assert(mInstance);
  return mInstance;
}

EmuCpu *createInstance(char *module_name, main_func_ptr_t main_entry_point)
{
  mInstance = new EmuCpu(module_name, main_entry_point);
}
