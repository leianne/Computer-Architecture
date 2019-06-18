#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->RAM[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int operandA;
  int operandB;  
  int running = 1; // True until we get a HLT instruction
  unsigned char IR;
  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    int PC = cpu->PC;
    IR = cpu->RAM[PC];
    // 2. Figure out how many operands this next instruction requires
    int num_of_operands = cpu_ram_read(cpu);
    // 3. Get the appropriate value(s) of the operands following this instruction
    if ( num_of_operands == 2) {
      operandA = cpu->RAM[(cpu->PC) + 1];
      operandB = cpu->RAM[(cpu->PC) + 2];
    } else if (num_of_operands == 1) {
      operandA = cpu->RAM[(cpu->PC) + 1];
    }
    // 4. switch() over it to decide on a course of action.
    switch (IR) {
      case LDI:
      /* code */
        cpu->REG[operandA] = operandB;
        cpu->PC += num_of_operands + 1;
      break;
      case PRN:
        printf("%d\n", cpu->REG[operandA]);
        cpu->PC += num_of_operands +1;
        break;
      case HLT:
        exit(1);
        break;
      default:
        break;
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  *cpu->RAM = memset(cpu->RAM, 0, sizeof(cpu->RAM));
  *cpu->REG = memset(cpu->REG, 0, sizeof(cpu->REG));

}

int cpu_ram_read(struct cpu *cpu) 
{
  int curr = cpu->RAM[(cpu->PC)];
  int operandA = cpu->RAM[(cpu->PC) + 1];
  int operandB = cpu->RAM[(cpu->PC) + 2];
  if (curr >= 128) {
    return 2;
  } else if (curr >= 64) {
    return 1;
  } else {
    return 0;
  }
}

void  cpu_ram_write(struct cpu *cpu)
{

}