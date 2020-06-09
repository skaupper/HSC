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
typedef void (*isr_func_ptr_t)(void*);

SC_MODULE(EmuCpu)
{
  /* initiator socket, with default settings */
  tlm_utils::simple_initiator_socket<EmuCpu> mSocket;

  static EmuCpu *getInstance();
  static EmuCpu *createInstance(char const *module_name, main_func_ptr_t main_entry_point);

  void setIsrCallback(isr_func_ptr_t isr_func);

  void read_bus(uint32_t addr, uint32_t * rd_data);
  void write_bus(uint32_t addr, uint32_t wr_data);

  sc_in<bool> iIrq;

private:
  SC_HAS_PROCESS(EmuCpu);
  EmuCpu(sc_module_name module_name, main_func_ptr_t main_entry_point);
  EmuCpu(const EmuCpu &) = delete;
  EmuCpu &operator=(const EmuCpu &) = delete;

  void run();
  void ISR();
  void doTransaction(tlm::tlm_command cmd, uint32_t addr, uint32_t * data);

  static EmuCpu *mInstance;
  main_func_ptr_t mMainFuncPtr;
  isr_func_ptr_t mIsrFunc;
};

#endif /* _EMU_CPU_H_ */
