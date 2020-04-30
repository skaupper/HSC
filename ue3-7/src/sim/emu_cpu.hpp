/*****************************************************************************/
/**
 * @file    emu_cpu.hpp
 * @author  Michael Wurm <michael.wurm@students.fh-hagenberg.at>
 *          Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
 * @brief   EmuCPU header file
 */
/*****************************************************************************/

#ifndef _EMU_CPU_H_
#define _EMU_CPU_H_

#include <systemc.h>
#include <tlm_utils/simple_initiator_socket.h>

typedef int (*main_func_ptr_t)();

SC_MODULE(EmuCpu)
{
  /* initiator socket, with default settings */
  tlm_utils::simple_initiator_socket<EmuCpu> mSocket;

  EmuCpu *getInstance();
  EmuCpu *createInstance(char *module_name, main_func_ptr_t main_entry_point);

  void write_bus(uint32_t addr, uint32_t data);
  void read_bus(uint32_t addr, uint32_t * data);

private:
  SC_HAS_PROCESS(EmuCpu);

  EmuCpu(sc_module_name module_name, main_func_ptr_t main_entry_point);
  EmuCpu(const EmuCpu &) = delete;

  EmuCpu *mInstance;
  main_func_ptr_t mMainFuncPtr;
};

#endif /* _EMU_CPU_H_ */
