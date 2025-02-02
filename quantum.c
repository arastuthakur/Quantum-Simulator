#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "quantum.h"

#define PI 3.14159265358979323846

QuantumState* create_quantum_state(int num_qubits) {
    if (num_qubits > MAX_QUBITS) {
        fprintf(stderr, "Error: Too many qubits requested\n");
        return NULL;
    }

    QuantumState* state = malloc(sizeof(QuantumState));
    state->num_qubits = num_qubits;
    state->state_size = 1 << num_qubits;  // 2^num_qubits
    state->amplitudes = calloc(state->state_size, sizeof(ComplexNum));
    
    // Initialize to |0> state
    state->amplitudes[0] = 1.0 + 0.0*I;
    
    return state;
}

void destroy_quantum_state(QuantumState* state) {
    free(state->amplitudes);
    free(state);
}

void normalize_state(QuantumState* state) {
    double norm = 0.0;
    for (int i = 0; i < state->state_size; i++) {
        norm += cabs(state->amplitudes[i]) * cabs(state->amplitudes[i]);
    }
    norm = sqrt(norm);
    
    for (int i = 0; i < state->state_size; i++) {
        state->amplitudes[i] /= norm;
    }
}

void apply_hadamard(QuantumState* state, int target_qubit) {
    ComplexNum* new_amplitudes = calloc(state->state_size, sizeof(ComplexNum));
    int mask = 1 << target_qubit;
    double scale = 1.0 / sqrt(2.0);
    
    for (int i = 0; i < state->state_size; i++) {
        int i0 = i & ~mask;  // i with target bit = 0
        int i1 = i | mask;   // i with target bit = 1
        
        if ((i & mask) == 0) {
            new_amplitudes[i0] += scale * state->amplitudes[i];
            new_amplitudes[i1] += scale * state->amplitudes[i];
        } else {
            new_amplitudes[i0] += scale * state->amplitudes[i];
            new_amplitudes[i1] -= scale * state->amplitudes[i];
        }
    }
    
    free(state->amplitudes);
    state->amplitudes = new_amplitudes;
}

void apply_pauli_x(QuantumState* state, int target_qubit) {
    int mask = 1 << target_qubit;
    for (int i = 0; i < state->state_size; i += 2) {
        ComplexNum temp = state->amplitudes[i];
        state->amplitudes[i] = state->amplitudes[i ^ mask];
        state->amplitudes[i ^ mask] = temp;
    }
}

void apply_pauli_z(QuantumState* state, int target_qubit) {
    int mask = 1 << target_qubit;
    for (int i = 0; i < state->state_size; i++) {
        if (i & mask) {
            state->amplitudes[i] *= -1;
        }
    }
}

void apply_pauli_y(QuantumState* state, int target_qubit) {
    int mask = 1 << target_qubit;
    ComplexNum i_unit = I;  // Imaginary unit
    
    for (int i = 0; i < state->state_size; i++) {
        if (i & mask) {
            state->amplitudes[i ^ mask] = i_unit * state->amplitudes[i];
            state->amplitudes[i] = -i_unit * state->amplitudes[i ^ mask];
        }
    }
}

void apply_phase(QuantumState* state, int target_qubit, double angle) {
    int mask = 1 << target_qubit;
    ComplexNum phase = cos(angle) + I * sin(angle);
    
    for (int i = 0; i < state->state_size; i++) {
        if (i & mask) {
            state->amplitudes[i] *= phase;
        }
    }
}

void apply_cnot(QuantumState* state, int control_qubit, int target_qubit) {
    int control_mask = 1 << control_qubit;
    int target_mask = 1 << target_qubit;
    
    for (int i = 0; i < state->state_size; i++) {
        if (i & control_mask) {  // If control qubit is 1
            int j = i ^ target_mask;  // Flip target qubit
            ComplexNum temp = state->amplitudes[i];
            state->amplitudes[i] = state->amplitudes[j];
            state->amplitudes[j] = temp;
        }
    }
}

void apply_swap(QuantumState* state, int qubit1, int qubit2) {
    int mask1 = 1 << qubit1;
    int mask2 = 1 << qubit2;
    
    for (int i = 0; i < state->state_size; i++) {
        int bit1 = (i & mask1) != 0;
        int bit2 = (i & mask2) != 0;
        
        if (bit1 != bit2) {
            int j = i ^ mask1 ^ mask2;  // Flip both bits
            ComplexNum temp = state->amplitudes[i];
            state->amplitudes[i] = state->amplitudes[j];
            state->amplitudes[j] = temp;
        }
    }
}

void apply_toffoli(QuantumState* state, int control1, int control2, int target) {
    int c1_mask = 1 << control1;
    int c2_mask = 1 << control2;
    int target_mask = 1 << target;
    
    for (int i = 0; i < state->state_size; i++) {
        if ((i & c1_mask) && (i & c2_mask)) {  // Both controls are 1
            int j = i ^ target_mask;  // Flip target bit
            ComplexNum temp = state->amplitudes[i];
            state->amplitudes[i] = state->amplitudes[j];
            state->amplitudes[j] = temp;
        }
    }
}

int measure_qubit(QuantumState* state, int qubit) {
    double prob_0 = 0.0;
    int mask = 1 << qubit;
    
    // Calculate probability of measuring |0>
    for (int i = 0; i < state->state_size; i++) {
        if ((i & mask) == 0) {
            prob_0 += cabs(state->amplitudes[i]) * cabs(state->amplitudes[i]);
        }
    }
    
    // Random measurement
    double rand_val = (double)rand() / RAND_MAX;
    int result = (rand_val > prob_0) ? 1 : 0;
    
    // Collapse state
    for (int i = 0; i < state->state_size; i++) {
        if ((i & mask) != (result << qubit)) {
            state->amplitudes[i] = 0;
        }
    }
    
    normalize_state(state);
    return result;
}

void grover_oracle(QuantumState* state, int marked_state) {
    // Phase flip for marked state
    state->amplitudes[marked_state] *= -1;
}

void grover_diffusion(QuantumState* state) {
    // Apply H gates to all qubits
    for (int i = 0; i < state->num_qubits; i++) {
        apply_hadamard(state, i);
    }
    
    // Apply phase flip to |0> state
    state->amplitudes[0] *= -1;
    
    // Apply H gates again
    for (int i = 0; i < state->num_qubits; i++) {
        apply_hadamard(state, i);
    }
}

void grover_search(QuantumState* state, int marked_state) {
    // Initialize with superposition
    for (int i = 0; i < state->num_qubits; i++) {
        apply_hadamard(state, i);
    }
    
    // Number of Grover iterations
    int iterations = (int)(PI / 4.0 * sqrt(state->state_size));
    
    // Apply Grover iteration
    for (int i = 0; i < iterations; i++) {
        grover_oracle(state, marked_state);
        grover_diffusion(state);
    }
}

void quantum_fourier_transform(QuantumState* state) {
    // Apply QFT to all qubits
    for (int i = 0; i < state->num_qubits; i++) {
        apply_hadamard(state, i);
        
        // Apply controlled phase rotations
        for (int j = i + 1; j < state->num_qubits; j++) {
            double angle = PI / pow(2, j - i);
            apply_phase(state, j, angle);
        }
    }
    
    // Reverse qubit order
    for (int i = 0; i < state->num_qubits / 2; i++) {
        apply_swap(state, i, state->num_qubits - 1 - i);
    }
}

void deutsch_jozsa(QuantumState* state, bool is_constant) {
    // Initialize state
    for (int i = 0; i < state->num_qubits; i++) {
        apply_hadamard(state, i);
    }
    
    // Apply oracle
    if (!is_constant) {
        // Balanced function: flip half of the outputs
        for (int i = 0; i < state->state_size / 2; i++) {
            apply_pauli_z(state, i);
        }
    }
    
    // Final Hadamard gates
    for (int i = 0; i < state->num_qubits; i++) {
        apply_hadamard(state, i);
    }
}

void quantum_teleportation(QuantumState* source, QuantumState* target, int source_qubit, int target_qubit) {
    // Create Bell pair between source and target
    apply_hadamard(target, target_qubit);
    apply_cnot(target, target_qubit, source_qubit);
    
    // Bell measurement on source
    apply_cnot(source, source_qubit, target_qubit);
    apply_hadamard(source, source_qubit);
    
    int m1 = measure_qubit(source, source_qubit);
    int m2 = measure_qubit(source, target_qubit);
    
    // Apply corrections based on measurements
    if (m2) apply_pauli_x(target, target_qubit);
    if (m1) apply_pauli_z(target, target_qubit);
}

void apply_controlled_phase(QuantumState* state, int control_qubit, int target_qubit, double angle) {
    int control_mask = 1 << control_qubit;
    int target_mask = 1 << target_qubit;
    ComplexNum phase = cos(angle) + I * sin(angle);
    
    for (int i = 0; i < state->state_size; i++) {
        if ((i & control_mask) && (i & target_mask)) {
            state->amplitudes[i] *= phase;
        }
    }
}

void apply_rotation_x(QuantumState* state, int target_qubit, double angle) {
    int mask = 1 << target_qubit;
    double cos_half = cos(angle/2);
    double sin_half = sin(angle/2);
    
    for (int i = 0; i < state->state_size; i++) {
        int i0 = i & ~mask;
        int i1 = i | mask;
        ComplexNum temp = state->amplitudes[i0];
        state->amplitudes[i0] = cos_half * temp - I * sin_half * state->amplitudes[i1];
        state->amplitudes[i1] = -I * sin_half * temp + cos_half * state->amplitudes[i1];
    }
}

void apply_rotation_y(QuantumState* state, int target_qubit, double angle) {
    int mask = 1 << target_qubit;
    double cos_half = cos(angle/2);
    double sin_half = sin(angle/2);
    
    for (int i = 0; i < state->state_size; i++) {
        int i0 = i & ~mask;
        int i1 = i | mask;
        ComplexNum temp = state->amplitudes[i0];
        state->amplitudes[i0] = cos_half * temp - sin_half * state->amplitudes[i1];
        state->amplitudes[i1] = sin_half * temp + cos_half * state->amplitudes[i1];
    }
}

void apply_rotation_z(QuantumState* state, int target_qubit, double angle) {
    apply_phase(state, target_qubit, angle);
}

void create_bell_pair(QuantumState* state, int qubit1, int qubit2) {
    apply_hadamard(state, qubit1);
    apply_cnot(state, qubit1, qubit2);
}

void apply_error_correction_encoding(QuantumState* state, int logical_qubit) {
    // Implement 3-qubit bit flip code
    int base_qubit = logical_qubit * 3;
    apply_cnot(state, base_qubit, base_qubit + 1);
    apply_cnot(state, base_qubit, base_qubit + 2);
}

void apply_error_correction_syndrome(QuantumState* state, int logical_qubit, int* syndrome) {
    // Measure error syndrome for 3-qubit code
    int base_qubit = logical_qubit * 3;
    syndrome[0] = measure_qubit(state, base_qubit) ^ measure_qubit(state, base_qubit + 1);
    syndrome[1] = measure_qubit(state, base_qubit) ^ measure_qubit(state, base_qubit + 2);
}

void apply_error_correction_recovery(QuantumState* state, int logical_qubit, int* syndrome) {
    int base_qubit = logical_qubit * 3;
    if (syndrome[0] && !syndrome[1]) {
        apply_pauli_x(state, base_qubit + 1);
    } else if (!syndrome[0] && syndrome[1]) {
        apply_pauli_x(state, base_qubit + 2);
    } else if (syndrome[0] && syndrome[1]) {
        apply_pauli_x(state, base_qubit);
    }
}

void quantum_random_number(QuantumState* state, int num_bits, int* result) {
    *result = 0;
    for (int i = 0; i < num_bits; i++) {
        apply_hadamard(state, i);
        int bit = measure_qubit(state, i);
        *result |= (bit << i);
    }
}

void quantum_walk_1d(QuantumState* state, int steps) {
    // Quantum walk on a line
    for (int step = 0; step < steps; step++) {
        // Coin flip (Hadamard)
        apply_hadamard(state, 0);
        
        // Conditional shift
        for (int i = 1; i < state->num_qubits; i++) {
            apply_controlled_phase(state, 0, i, PI/2);
        }
    }
}

void quantum_phase_estimation(QuantumState* state, double true_phase) {
    int precision_qubits = state->num_qubits / 2;
    int target_qubit = state->num_qubits - 1;
    
    // Initialize phase estimation qubits
    for (int i = 0; i < precision_qubits; i++) {
        apply_hadamard(state, i);
    }
    
    // Apply controlled rotations
    for (int i = 0; i < precision_qubits; i++) {
        double angle = true_phase * pow(2, i);
        apply_controlled_phase(state, i, target_qubit, angle);
    }
    
    // Inverse QFT
    quantum_fourier_transform(state);
}

void shor_period_finding(QuantumState* state, int number_to_factor, int* period) {
    // Simplified implementation for demonstration
    int register_size = state->num_qubits / 2;
    
    // Initialize first register in superposition
    for (int i = 0; i < register_size; i++) {
        apply_hadamard(state, i);
    }
    
    // Apply modular exponentiation (simplified)
    for (int i = 0; i < register_size; i++) {
        apply_controlled_phase(state, i, register_size + i, 2 * PI / number_to_factor);
    }
    
    // QFT on first register
    quantum_fourier_transform(state);
    
    // Measure to find period (simplified)
    *period = 0;
    for (int i = 0; i < register_size; i++) {
        *period |= (measure_qubit(state, i) << i);
    }
}
