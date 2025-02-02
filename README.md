# Quantum Computing Simulator

An interactive quantum computing simulator implemented in C that demonstrates various quantum phenomena, algorithms, and applications. This simulator provides hands-on experience with quantum computing concepts through a user-friendly interface.

## Features

### Basic Quantum Operations
- Single-qubit gates (Hadamard, X/NOT, Y, Z)
- Two-qubit gates (CNOT, SWAP)
- Three-qubit gates (Toffoli/CCNOT)
- Phase and rotation gates (Rx, Ry, Rz)
- Measurement operations

### Quantum Algorithms
1. **Grover's Search Algorithm**
   - Search in unstructured databases
   - Demonstrates quantum speedup
   - Configurable number of qubits and target states

2. **Deutsch-Jozsa Algorithm**
   - Determines if a function is constant or balanced
   - Multiple oracle function types
   - Shows quantum parallelism

3. **Quantum Fourier Transform**
   - Transforms quantum states to frequency domain
   - Multiple input state options
   - Custom state preparation

4. **Quantum Teleportation**
   - Transfer quantum states between qubits
   - Demonstrates quantum entanglement
   - Multiple initial state options
   - Custom state angles

5. **Quantum Error Correction**
   - 3-qubit bit flip code
   - Error simulation and detection
   - Recovery operations
   - Interactive error injection

6. **Quantum Random Number Generator**
   - True quantum randomness
   - Configurable bit length (1-8 bits)
   - Binary and decimal output
   - Based on quantum superposition

7. **Quantum Walk**
   - 1D quantum walk simulation
   - Configurable number of steps and positions
   - Demonstrates quantum interference
   - Compare with classical random walks

8. **Quantum Phase Estimation**
   - Estimate unknown quantum phases
   - Configurable precision
   - Uses quantum Fourier transform
   - Interactive phase input

9. **Shor's Period Finding**
   - Core component of Shor's factoring algorithm
   - Period finding in modular arithmetic
   - Demonstrates quantum advantage
   - Works with numbers 3-15

## Building and Running

### Prerequisites
- GCC compiler
- Math library (libm)
- C99 or later

### Compilation
```bash
gcc -o quantum_sim main.c quantum.c -lm
```

### Running
```bash
./quantum_sim
```

## Usage Guide

1. Launch the simulator using the command above
2. Choose from the available quantum experiments (1-10)
3. Follow the interactive prompts to:
   - Set number of qubits
   - Choose initial states
   - Configure algorithm parameters
   - View quantum state evolution
   - Measure final results

## Interactive Features
- User-friendly menu system
- Input validation for all parameters
- Clear visualization of quantum states
- Step-by-step execution
- Error handling and default values
- "Press Enter to continue" between experiments

## Implementation Details

### Quantum State Representation
- Complex amplitudes for quantum states
- Efficient state vector manipulation
- Support for up to 16 qubits
- Automatic state normalization

### Quantum Gates
- Basic gates (H, X, Y, Z)
- Controlled operations
- Rotation gates with arbitrary angles
- Multi-qubit entangling operations

### Error Handling
- Input validation for all user inputs
- Graceful fallback to default values
- Clear error messages
- Memory management for quantum states

## Examples

### Quantum Teleportation
```
1. Choose initial state (|0>, |1>, |+>, or custom)
2. Watch state preparation
3. Observe teleportation protocol
4. Verify final state
```

### Grover's Search
```
1. Select number of qubits (2-4)
2. Choose target state
3. Watch amplitude amplification
4. Measure final state
```

### Quantum Random Numbers
```
1. Choose number of bits (1-8)
2. Generate quantum randomness
3. View both binary and decimal results
```

## Contributing
Feel free to contribute to this project by:
- Adding new quantum algorithms
- Improving simulation efficiency
- Enhancing the user interface
- Adding visualization features

## License
This project is open source and available under the MIT License.

## Authors
Created by Arastu Thakur
