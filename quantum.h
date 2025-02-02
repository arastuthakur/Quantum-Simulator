#ifndef QUANTUM_H
#define QUANTUM_H

#include <complex.h>
#include <stdbool.h>

#define MAX_QUBITS 16

// Complex number type for quantum amplitudes
typedef double complex ComplexNum;

// Quantum state structure
typedef struct {
    int num_qubits;
    int state_size;
    ComplexNum* amplitudes;
} QuantumState;

// Basic quantum gates
typedef enum {
    HADAMARD,
    PAULI_X,
    PAULI_Y,
    PAULI_Z,
    PHASE,
    CNOT,
    SWAP,
    TOFFOLI
} GateType;

// Function prototypes
QuantumState* create_quantum_state(int num_qubits);
void destroy_quantum_state(QuantumState* state);

// Single qubit gates
void apply_hadamard(QuantumState* state, int target_qubit);
void apply_pauli_x(QuantumState* state, int target_qubit);
void apply_pauli_y(QuantumState* state, int target_qubit);
void apply_pauli_z(QuantumState* state, int target_qubit);
void apply_phase(QuantumState* state, int target_qubit, double angle);

// Two qubit gates
void apply_cnot(QuantumState* state, int control_qubit, int target_qubit);
void apply_swap(QuantumState* state, int qubit1, int qubit2);
void apply_toffoli(QuantumState* state, int control1, int control2, int target);

// Additional quantum gates
void apply_controlled_phase(QuantumState* state, int control_qubit, int target_qubit, double angle);
void apply_rotation_x(QuantumState* state, int target_qubit, double angle);
void apply_rotation_y(QuantumState* state, int target_qubit, double angle);
void apply_rotation_z(QuantumState* state, int target_qubit, double angle);

// Measurement
int measure_qubit(QuantumState* state, int qubit);
void normalize_state(QuantumState* state);

// Grover's algorithm
void grover_search(QuantumState* state, int marked_state);
void grover_diffusion(QuantumState* state);
void grover_oracle(QuantumState* state, int marked_state);

// Quantum algorithms
void quantum_fourier_transform(QuantumState* state);
void deutsch_jozsa(QuantumState* state, bool is_constant);

// Advanced quantum operations
void quantum_teleportation(QuantumState* source, QuantumState* target, int source_qubit, int target_qubit);
void create_bell_pair(QuantumState* state, int qubit1, int qubit2);
void apply_error_correction_encoding(QuantumState* state, int logical_qubit);
void apply_error_correction_syndrome(QuantumState* state, int logical_qubit, int* syndrome);
void apply_error_correction_recovery(QuantumState* state, int logical_qubit, int* syndrome);
void quantum_random_number(QuantumState* state, int num_bits, int* result);
void quantum_walk_1d(QuantumState* state, int steps);
void quantum_phase_estimation(QuantumState* state, double true_phase);
void shor_period_finding(QuantumState* state, int number_to_factor, int* period);

#endif /* QUANTUM_H */
