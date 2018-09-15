#include <stdio.h>
#include "vm.h"
#include "data.h"

/* ************************************************************************************ */
/* Declarations                                                                         */
/* ************************************************************************************ */

/**
 * Recommended design includes the following functions implemented.
 * However, you are free to change them as you wish inside the vm.c file.
 * */
void initVM(VirtualMachine*);

int readInstructions(FILE*, Instruction*);

void dumpInstructions(FILE*, Instruction*, int numOfIns);

int getBasePointer(int *stack, int currentBP, int L);

void dumpStack(FILE*, int* stack, int sp, int bp);

int executeInstruction(VirtualMachine* vm, Instruction ins, FILE* vmIn, FILE* vmOut);

int base(int l, int base);


/* ************************************************************************************ */
/* Global Data and misc structs & enums                                                 */
/* ************************************************************************************ */

/**
 * allows conversion from opcode to opcode string
 * */
const char *opcodes[] = 
{
    "illegal", // opcode 0 is illegal
    "lit", "rtn", "lod", "sto", "cal", // 1, 2, 3 ..
    "inc", "jmp", "jpc", "sio", "sio",
    "sio", "neg", "add", "sub", "mul",
    "div", "odd", "mod", "eql", "neq",
    "lss", "leq", "gtr", "geq"
};

enum { CONT, HALT };

/* ************************************************************************************ */
/* Definitions                                                                          */
/* ************************************************************************************ */

/**
 * Initialize Virtual Machine
 * */
void initVM(VirtualMachine* vm)
{
    if(vm)
    {
        // TODO
    }
}

/**
 * Fill the (ins)tructions array by reading instructions from (in)put file
 * Return the number of instructions read
 * */
int readInstructions(FILE* in, Instruction* ins)
{
    // Instruction index
    int i = 0;
    
    while(fscanf(in, "%d %d %d %d", &ins[i].op, &ins[i].r, &ins[i].l, &ins[i].m) != EOF)
    {
        i++;
    }

    // Return the number of instructions read
    return i;
}

/**
 * Dump instructions to the output file
 * */
void dumpInstructions(FILE* out, Instruction* ins, int numOfIns)
{
    // Header
    fprintf(out,
        "***Code Memory***\n%3s %3s %3s %3s %3s \n",
        "#", "OP", "R", "L", "M"
        );

    // Instructions
    int i;
    for(i = 0; i < numOfIns; i++)
    {
        fprintf(
            out,
            "%3d %3s %3d %3d %3d \n", // formatting
            i, opcodes[ins[i].op], ins[i].r, ins[i].l, ins[i].m
        );
    }
}

/**
 * Returns the base pointer for the lexiographic level L
 * */
int getBasePointer(int *stack, int currentBP, int L)
{
    // TODO
}

// Function that dumps the whole stack into output file
// Do not forget to use '|' character between stack frames
void dumpStack(FILE* out, int* stack, int sp, int bp)
{
    if(bp == 0)
        return;

    // bottom-most level, where a single zero value lies
    if(bp == 1)
    {
        fprintf(out, "%3d ", 0);
    }

    // former levels - if exists
    if(bp != 1)
    {
        dumpStack(out, stack, bp - 1, stack[bp + 2]);            
    }

    // top level: current activation record
    if(bp <= sp)
    {
        // indicate a new activation record
        fprintf(out, "| ");

        // print the activation record
        int i;
        for(i = bp; i <= sp; i++)
        {
            fprintf(out, "%3d ", stack[i]);
        }
    }
}

/**
 * Executes the (ins)truction on the (v)irtual (m)achine.
 * This changes the state of the virtual machine.
 * Returns HALT if the executed instruction was meant to halt the VM.
 * .. Otherwise, returns CONT
 * */
int executeInstruction(VirtualMachine* vm, Instruction ins, FILE* vmIn, FILE* vmOut)
{
    switch(ins.op)
    {
        // TODO
        
        case 1: // LIT R 0 M
        {
            vm->RF[ins.r] = ins.m;
            break;
        }
        case 2: // RTN 0 0 0
        {
            vm->SP = vm->BP - 1;
            vm->BP = vm->stack[vm->SP + 3];
            vm->PC = vm->stack[vm->SP + 4];
            break;
        }
        case 3: // LOD R L M
        {
            vm->RF[ins.r] = vm->stack[base(ins.l, vm->BP) + ins.m];
            break;
        }
        
        case 4: // STORE FUNCTION, WORK ON BP
        {
            vm->stack[base(ins.l, vm->BP) + ins.m] = vm->RF[ins.r];
            break;
        }
        
        
        case 5: // CALL TO A FUNCTION [CAL 0, L, M]
        {
            vm->stack[vm->SP + 1] = 0;
            vm->stack[vm->SP + 2] = base(ins.l, vm->BP);
            vm->stack[vm->SP + 3] = vm->BP;
            vm->stack[vm->SP + 4] = vm->PC;
            vm->BP = vm->SP + 1;
            vm->PC = ins.m;
            break;
        }
        
        case 6: // [INC X, X, X]
        {
            vm->SP = vm->SP + ins.m;
            break;
        }
        
        case 7: // [JMP X, X, X] JUMP FUNCTION!!!
        {
            vm->PC = ins.m;
            break;
        }
        
        case 8: // [JPC X, X, X] JUMP POINTER FUNCTION!!!
        {
            if (vm->RF[ins.r] == 0)
            {
                vm->PC = ins.m;
            }
            break;
        }
        
        case 9: // [SIO R, X, 1]
        {
            fprintf(vmOut, "%d ", vm->RF[ins.r]);
            break;
        }
        
        case 10: // [SIO R, X, 2] // WORK ON THIS RED FUNCTION
        {
            vm->PC = ins.m;
            break;
        }
        
        case 11: // [SIO R, X, 2]
        {
            return HALT;
            break;
        }
        
        case 12: // NEGATION;
        {
            vm->RF[ins.r] = -(vm->RF[ins.l]);
            break;
        }
        
        case 13: // ADDITION;
        {
            vm->RF[ins.r] =  vm->RF[ins.l] + vm->RF[ins.m];
            break;
        }
        
        case 14: // SUBTRACTION;
        {
            vm->RF[ins.r] =  vm->RF[ins.l] - vm->RF[ins.m];
            break;
        }
        
        case 15: // MULTIPLICATION;
        {
            vm->RF[ins.r] =  vm->RF[ins.l] * vm->RF[ins.m];
            break;
        }
        
        case 16: // DIVISION;
        {
            vm->RF[ins.r] =  vm->RF[ins.l] / vm->RF[ins.m];
            break;
        }
        
        case 17: // ODD NUMBER;
        {
            vm->RF[ins.r] =  vm->RF[ins.r] % 2;
            break;
        }
        
        case 18: // MODULO;
        {
            vm->RF[ins.r] =  vm->RF[ins.l] % vm->RF[ins.m];
            break;
        }
        
        case 19: // BRANCH EQUAL;
        {
            if (vm->RF[ins.l] ==  vm->RF[ins.m])
            {
                vm->RF[ins.r] = 1;
            }
            
            else
                vm->RF[ins.r] = 0;
            break;
        }
        
        case 20: // BRANCH NOT EQUAL;
        {
            if (vm->RF[ins.l] !=  vm->RF[ins.m])
            {
                vm->RF[ins.r] = 1;
            }
            
            else
                vm->RF[ins.r] = 0;
            break;
        }
        
        case 21: // BRANCH LESS;
        {
            if (vm->RF[ins.l] <  vm->RF[ins.m])
            {
                vm->RF[ins.r] = 1;
            }
            
            else
            {
                vm->RF[ins.r] = 0;
            }
            break;
        }
        
        case 22: // BRANCH LESS EQUAL TO;
        {
            if (vm->RF[ins.l] <=  vm->RF[ins.m])
            {
                vm->RF[ins.r] = 1;
            }
            
            else
            {
                vm->RF[ins.r] = 0;
            }
            break;
        }
        
        case 23: // BRANCH GREATER;
        {
            if (vm->RF[ins.l] >  vm->RF[ins.m])
            {
                vm->RF[ins.r] = 1;
            }
            
            else
            {
                vm->RF[ins.r] = 0;
            }
            break;
        }
        
        case 24: // BRANCH GREATER EQ;
        {
            if (vm->RF[ins.l] >=  vm->RF[ins.m])
            {
                vm->RF[ins.r] = 1;
            }
            
            else
            {
                vm->RF[ins.r] = 0;
            }
            break;
        }
        
        default:
            fprintf(stderr, "Illegal instruction?");
            return HALT;
    }

    return CONT;
}

/**
 * inp: The FILE pointer containing the list of instructions to
 *         be loaded to code memory of the virtual machine.
 * 
 * outp: The FILE pointer to write the simulation output, which
 *       contains both code memory and execution history.
 * 
 * vm_inp: The FILE pointer that is going to be attached as the input
 *         stream to the virtual machine. Useful to feed input for SIO
 *         instructions.
 * 
 * vm_outp: The FILE pointer that is going to be attached as the output
 *          stream to the virtual machine. Useful to save the output printed
 *          by SIO instructions.
 * */
void simulateVM(
    FILE* inp,
    FILE* outp,
    FILE* vm_inp,
    FILE* vm_outp
    )
{
    // Read instructions from file
    // TODO

    // Dump instructions to the output file
    // TODO

    // Before starting the code execution on the virtual machine,
    // .. write the header for the simulation part (***Execution***)
    fprintf(outp, "\n***Execution***\n");
    fprintf(
        outp,
        "%3s %3s %3s %3s %3s %3s %3s %3s %3s \n",         // formatting
        "#", "OP", "R", "L", "M", "PC", "BP", "SP", "STK" // titles
    );

    // Create a virtual machine
    // TODO

    // Initialize the virtual machine
    // TODO

    // Fetch&Execute the instructions on the virtual machine until halting
    while( 1 /* TODO: Until halt is signalled.. */ )
    {
        // Fetch
        // TODO

        // Advance PC - before execution!
        // TODO

        // Execute the instruction
        // TODO

        // Print current state
        // TODO: Following is a possible way of printing the current state
        // .. where instrBeingExecuted is the address of the instruction at vm
        // ..  memory and instr is the instruction being executed.
        /* fprintf(
            outp,
            "%3d %3s %3d %3d %3d %3d %3d %3d ",
            instrBeingExecuted, // place of instruction at memory
             opcodes[instr.op], instr.r, instr.l, instr.m, // instruction info
             vm.PC, vm.BP, vm.SP // vm info
        );*/

        // Print stack info
        // TODO

        fprintf(outp, "\n");
    }

    // Above loop ends when machine halts. Therefore, dump halt message.
    fprintf(outp, "HLT\n");
    return;
}

int base (int l, int base)
{
    int b1;
    b1 = base;
    while (l > 0)
    {
        b1 = vm->stack [ b1 +1];
        l--;
    }
    return b1;
}
