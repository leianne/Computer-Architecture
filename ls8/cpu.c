#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

#define DATA_LEN 6
#define SP 7

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
int cpu_ram_read(struct cpu *cpu, unsigned char address) 
{
  return cpu->RAM[address];
}

void  cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char val)
{
  cpu->RAM[address] = val;
}

void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp;
  char fcontent[1024];
  int address = 0;

  fp = fopen(filename , "r");
  if (fp == NULL) {
    printf("Cannot find that file\n");
    exit(1);
  } 
  while(fgets(fcontent, 1024, fp) != NULL) {
    char *endptr;
    unsigned  char v;
    v = strtoul(fcontent, &endptr, 2);

    if (endptr == fcontent) {
      continue;
    }
    cpu_ram_write(cpu, address++,  v);
  };
  fclose(fp);
  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case MUL:
      // TODO
      cpu->REG[regA] = cpu->REG[regA]  * cpu->REG[regB];
      break;
    case ADD:
      cpu->REG[regA] = cpu->REG[regA] + cpu->REG[regB];
      break;
    default:
      break;
    // TODO: implement more ALU ops
  }
}

void push(struct cpu *cpu, unsigned char regVal)
{
  cpu->REG[SP]--;
  unsigned char val = cpu->REG[regVal];
  cpu_ram_write(cpu, cpu->REG[7], val);
  cpu->PC += 2;
}

void pop(struct cpu *cpu, unsigned char regVal)
{

  unsigned char val = cpu_ram_read(cpu, cpu->REG[SP]);
  cpu->REG[regVal] = val;
  cpu->REG[SP]++;
  cpu->PC += 2;
}


void call(struct cpu *cpu, unsigned char regVal)
{
  unsigned char address = cpu->REG[regVal];
  cpu->REG[SP]--;
  cpu_ram_write(cpu, cpu->REG[SP], cpu->PC + 2);
  cpu->PC = address;
}

void ret(struct cpu *cpu)
{
  unsigned char address = cpu_ram_read(cpu, cpu->REG[SP]);  // increment size of stack, because we pop out of it
  cpu->REG[SP]++;
  cpu->PC = address;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  unsigned  char operandA, operandB;
  int running = 1; // True until we get a HLT instruction
  unsigned char IR;
  int reg, v, num_of_operands;
  cpu->REG[SP] = 120;
  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    operandA = cpu_ram_read(cpu, cpu->PC + 1);
    operandB = cpu_ram_read(cpu, cpu->PC + 2);


    if(operandA && operandB) {
      num_of_operands = 2;
    } else if (operandA ||  operandB){
      num_of_operands = 1;
    } else {
      num_of_operands = 0;
    }
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    switch (IR) {
      case LDI:

      /* code */
        cpu->REG[operandA] = operandB;
        cpu->PC += 3;
      break;
      case PRN:
        printf("%d\n", cpu->REG[operandA]);
        cpu->PC += 2;
        break;
      case HLT:
        running = 0;
        exit(1);
        break;
      case MUL:
        alu(cpu, MUL, operandA, operandB);
        cpu->PC += 3;
      case ADD:
        alu(cpu, ADD, operandA, operandB);
        cpu->PC += 3;
        break;
      case POP:
        pop(cpu, operandA);
        break;
      case PUSH:
        push(cpu, operandA);
        break;
      case CALL:
        call(cpu, operandA);
        break;
      case RET:
        ret(cpu);
        break;
      default:
        break;
    }
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
  cpu->REG[SP] = 0xF4;
  cpu->FLAG = 0;
}
