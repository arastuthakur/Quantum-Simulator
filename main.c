#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "quantum.h"

#define PI 3.14159265358979323846
#define MAX_INPUT 100

void print_state(QuantumState* state) {
    printf("Quantum State:\n");
    for (int i = 0; i < state->state_size; i++) {
        if (cabs(state->amplitudes[i]) > 0.001) {
            printf("|%d>: %.3f + %.3fi\n", i, 
                   creal(state->amplitudes[i]), 
                   cimag(state->amplitudes[i]));
        }
    }
    printf("\n");
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

double get_phase_angle() {
    char input[MAX_INPUT];
    printf("Enter phase angle (options: pi/8, pi/4, pi/2, pi, 2pi): ");
    fgets(input, MAX_INPUT, stdin);
    input[strcspn(input, "\n")] = 0;
    
    if (strcmp(input, "pi/8") == 0) return PI/8;
    if (strcmp(input, "pi/4") == 0) return PI/4;
    if (strcmp(input, "pi/2") == 0) return PI/2;
    if (strcmp(input, "pi") == 0) return PI;
    if (strcmp(input, "2pi") == 0) return 2*PI;
    
    return PI/4;  // default value
}

void interactive_phase_gate() {
    printf("\n=== Phase Gate Experiment ===\n");
    int num_qubits;
    
    printf("Enter number of qubits (1-4): ");
    scanf("%d", &num_qubits);
    clear_input_buffer();
    
    if (num_qubits < 1 || num_qubits > 4) {
        printf("Invalid number of qubits. Using 1 qubit.\n");
        num_qubits = 1;
    }
    
    QuantumState* state = create_quantum_state(num_qubits);
    
    // Create initial superposition
    for (int i = 0; i < num_qubits; i++) {
        apply_hadamard(state, i);
    }
    
    printf("\nInitial superposition state:\n");
    print_state(state);
    
    double angle = get_phase_angle();
    printf("\nApplying Phase(%.2f):\n", angle);
    
    int target_qubit;
    printf("Enter target qubit (0-%d): ", num_qubits-1);
    scanf("%d", &target_qubit);
    clear_input_buffer();
    
    if (target_qubit >= 0 && target_qubit < num_qubits) {
        apply_phase(state, target_qubit, angle);
        print_state(state);
    } else {
        printf("Invalid target qubit.\n");
    }
    
    destroy_quantum_state(state);
}

void interactive_grovers_search() {
    printf("\n=== Grover's Search Algorithm ===\n");
    int num_qubits;
    
    printf("Enter number of qubits (2-4): ");
    scanf("%d", &num_qubits);
    clear_input_buffer();
    
    if (num_qubits < 2 || num_qubits > 4) {
        printf("Invalid number of qubits. Using 3 qubits.\n");
        num_qubits = 3;
    }
    
    int max_state = 1 << num_qubits;
    int marked_state;
    
    printf("Enter state to search for (0-%d): ", max_state-1);
    scanf("%d", &marked_state);
    clear_input_buffer();
    
    if (marked_state < 0 || marked_state >= max_state) {
        printf("Invalid state. Using state |0>.\n");
        marked_state = 0;
    }
    
    QuantumState* state = create_quantum_state(num_qubits);
    printf("\nSearching for state |%d>:\n", marked_state);
    
    grover_search(state, marked_state);
    
    printf("After Grover's algorithm:\n");
    print_state(state);
    
    printf("Measuring state...\n");
    int result = 0;
    for (int i = 0; i < num_qubits; i++) {
        int bit = measure_qubit(state, i);
        result |= (bit << i);
    }
    printf("Final measurement: |%d>\n", result);
    
    destroy_quantum_state(state);
}

void interactive_deutsch_jozsa() {
    printf("\n=== Deutsch-Jozsa Algorithm ===\n");
    printf("Select oracle function type:\n");
    printf("1. Constant-0\n");
    printf("2. Constant-1\n");
    printf("3. Balanced-A (half 0s, half 1s)\n");
    printf("4. Balanced-B (alternating 0s and 1s)\n");
    
    int choice;
    printf("Enter choice (1-4): ");
    scanf("%d", &choice);
    clear_input_buffer();
    
    if (choice < 1 || choice > 4) {
        printf("Invalid choice. Using Constant-0.\n");
        choice = 1;
    }
    
    QuantumState* state = create_quantum_state(3);
    printf("\nTesting selected function:\n");
    
    switch(choice) {
        case 1:
            deutsch_jozsa(state, true);
            break;
        case 2:
            apply_pauli_x(state, 0);
            deutsch_jozsa(state, true);
            break;
        case 3:
            deutsch_jozsa(state, false);
            break;
        case 4:
            apply_pauli_x(state, 1);
            deutsch_jozsa(state, false);
            break;
    }
    
    print_state(state);
    printf("If measured |0>, function is constant\n");
    printf("Otherwise, function is balanced\n");
    
    destroy_quantum_state(state);
}

void interactive_qft() {
    printf("\n=== Quantum Fourier Transform ===\n");
    printf("Select input state:\n");
    printf("1. |0>\n");
    printf("2. |1>\n");
    printf("3. |+> (Equal superposition)\n");
    printf("4. Custom state (specify bits)\n");
    
    int choice;
    printf("Enter choice (1-4): ");
    scanf("%d", &choice);
    clear_input_buffer();
    
    QuantumState* state = create_quantum_state(3);
    
    switch(choice) {
        case 2:
            apply_pauli_x(state, 0);
            break;
        case 3:
            for (int i = 0; i < 3; i++) {
                apply_hadamard(state, i);
            }
            break;
        case 4: {
            printf("Enter 3 bits (e.g., 101): ");
            char bits[4];
            fgets(bits, 4, stdin);
            clear_input_buffer();
            
            for (int i = 0; i < 3; i++) {
                if (bits[i] == '1') {
                    apply_pauli_x(state, i);
                }
            }
            break;
        }
    }
    
    printf("\nInitial state:\n");
    print_state(state);
    
    quantum_fourier_transform(state);
    
    printf("After QFT:\n");
    print_state(state);
    
    destroy_quantum_state(state);
}

void interactive_quantum_teleportation() {
    printf("\n=== Quantum Teleportation ===\n");
    
    // Create source and target states
    QuantumState* source = create_quantum_state(2);
    QuantumState* target = create_quantum_state(2);
    
    // Prepare source state
    printf("Preparing source qubit...\n");
    printf("1. |0>\n");
    printf("2. |1>\n");
    printf("3. |+> (Superposition)\n");
    printf("4. Custom state (specify angle)\n");
    
    int choice;
    printf("Choose initial state (1-4): ");
    scanf("%d", &choice);
    clear_input_buffer();
    
    switch(choice) {
        case 2:
            apply_pauli_x(source, 0);
            break;
        case 3:
            apply_hadamard(source, 0);
            break;
        case 4: {
            double angle;
            printf("Enter rotation angle (0-360 degrees): ");
            scanf("%f", &angle);
            clear_input_buffer();
            apply_rotation_y(source, 0, angle * PI / 180.0);
            break;
        }
    }
    
    printf("\nInitial source state:\n");
    print_state(source);
    
    // Perform teleportation
    printf("\nPerforming quantum teleportation...\n");
    quantum_teleportation(source, target, 0, 0);
    
    printf("\nFinal target state:\n");
    print_state(target);
    
    destroy_quantum_state(source);
    destroy_quantum_state(target);
}

void interactive_error_correction() {
    printf("\n=== Quantum Error Correction ===\n");
    QuantumState* state = create_quantum_state(3);
    
    // Prepare initial state
    printf("Preparing logical qubit...\n");
    printf("1. |0>\n");
    printf("2. |1>\n");
    printf("3. |+> (Superposition)\n");
    
    int choice;
    printf("Choose initial state (1-3): ");
    scanf("%d", &choice);
    clear_input_buffer();
    
    switch(choice) {
        case 2:
            apply_pauli_x(state, 0);
            break;
        case 3:
            apply_hadamard(state, 0);
            break;
    }
    
    printf("\nInitial state:\n");
    print_state(state);
    
    // Encode
    printf("\nEncoding state using 3-qubit code...\n");
    apply_error_correction_encoding(state, 0);
    print_state(state);
    
    // Simulate error
    printf("\nSimulating error...\n");
    printf("1. No error\n");
    printf("2. Bit flip on qubit 1\n");
    printf("3. Bit flip on qubit 2\n");
    printf("4. Bit flip on qubit 3\n");
    
    printf("Choose error type (1-4): ");
    scanf("%d", &choice);
    clear_input_buffer();
    
    if (choice > 1 && choice <= 4) {
        apply_pauli_x(state, choice - 2);
    }
    
    printf("\nState after error:\n");
    print_state(state);
    
    // Error correction
    printf("\nPerforming error correction...\n");
    int syndrome[2];
    apply_error_correction_syndrome(state, 0, syndrome);
    apply_error_correction_recovery(state, 0, syndrome);
    
    printf("\nCorrected state:\n");
    print_state(state);
    
    destroy_quantum_state(state);
}

void interactive_quantum_random() {
    printf("\n=== Quantum Random Number Generator ===\n");
    
    int num_bits;
    printf("Enter number of random bits (1-8): ");
    scanf("%d", &num_bits);
    clear_input_buffer();
    
    if (num_bits < 1 || num_bits > 8) {
        printf("Invalid number of bits. Using 4 bits.\n");
        num_bits = 4;
    }
    
    QuantumState* state = create_quantum_state(num_bits);
    int result;
    
    printf("\nGenerating %d random bits using quantum superposition...\n", num_bits);
    quantum_random_number(state, num_bits, &result);
    
    printf("Random number: %d (binary: ", result);
    for (int i = num_bits - 1; i >= 0; i--) {
        printf("%d", (result >> i) & 1);
    }
    printf(")\n");
    
    destroy_quantum_state(state);
}

void interactive_quantum_walk() {
    printf("\n=== Quantum Walk Simulation ===\n");
    
    int num_positions;
    printf("Enter number of positions (2-6): ");
    scanf("%d", &num_positions);
    clear_input_buffer();
    
    if (num_positions < 2 || num_positions > 6) {
        printf("Invalid number of positions. Using 4 positions.\n");
        num_positions = 4;
    }
    
    int num_steps;
    printf("Enter number of steps (1-10): ");
    scanf("%d", &num_steps);
    clear_input_buffer();
    
    if (num_steps < 1 || num_steps > 10) {
        printf("Invalid number of steps. Using 5 steps.\n");
        num_steps = 5;
    }
    
    QuantumState* state = create_quantum_state(num_positions);
    
    printf("\nInitial state (particle at position 0):\n");
    print_state(state);
    
    quantum_walk_1d(state, num_steps);
    
    printf("\nFinal state distribution:\n");
    print_state(state);
    
    destroy_quantum_state(state);
}

void interactive_phase_estimation() {
    printf("\n=== Quantum Phase Estimation ===\n");
    
    double true_phase;
    printf("Enter phase to estimate (0-1): ");
    scanf("%lf", &true_phase);
    clear_input_buffer();
    
    if (true_phase < 0 || true_phase > 1) {
        printf("Invalid phase. Using 0.25.\n");
        true_phase = 0.25;
    }
    
    int precision_qubits = 4;
    QuantumState* state = create_quantum_state(precision_qubits + 1);
    
    printf("\nEstimating phase %.4f...\n", true_phase);
    quantum_phase_estimation(state, true_phase * 2 * PI);
    
    printf("\nFinal state (phase register):\n");
    print_state(state);
    
    destroy_quantum_state(state);
}

void interactive_shor() {
    printf("\n=== Shor's Period Finding Algorithm ===\n");
    
    int number;
    printf("Enter number to factor (3-15): ");
    scanf("%d", &number);
    clear_input_buffer();
    
    if (number < 3 || number > 15) {
        printf("Invalid number. Using 15.\n");
        number = 15;
    }
    
    QuantumState* state = create_quantum_state(8);  // 4 qubits each for input and output registers
    int period;
    
    printf("\nFinding period for function f(x) = a^x mod %d...\n", number);
    shor_period_finding(state, number, &period);
    
    printf("Found period: %d\n", period);
    if (period > 0 && period % 2 == 0) {
        int factor1 = (int)pow(2, period/2) + 1;
        int factor2 = (int)pow(2, period/2) - 1;
        printf("Potential factors: %d and %d\n", 
               factor1 % number, factor2 % number);
    }
    
    destroy_quantum_state(state);
}

void print_menu() {
    printf("\n=== Quantum Computing Simulator ===\n");
    printf("1. Phase Gate Experiment\n");
    printf("2. Grover's Search Algorithm\n");
    printf("3. Deutsch-Jozsa Algorithm\n");
    printf("4. Quantum Fourier Transform\n");
    printf("5. Quantum Teleportation\n");
    printf("6. Quantum Error Correction\n");
    printf("7. Quantum Random Number Generator\n");
    printf("8. Quantum Walk Simulation\n");
    printf("9. Quantum Phase Estimation\n");
    printf("10. Shor's Period Finding\n");
    printf("11. Exit\n");
    printf("Enter choice (1-11): ");
}

int main() {
    srand(time(NULL));
    
    while (1) {
        print_menu();
        int choice;
        scanf("%d", &choice);
        clear_input_buffer();
        
        switch(choice) {
            case 1:
                interactive_phase_gate();
                break;
            case 2:
                interactive_grovers_search();
                break;
            case 3:
                interactive_deutsch_jozsa();
                break;
            case 4:
                interactive_qft();
                break;
            case 5:
                interactive_quantum_teleportation();
                break;
            case 6:
                interactive_error_correction();
                break;
            case 7:
                interactive_quantum_random();
                break;
            case 8:
                interactive_quantum_walk();
                break;
            case 9:
                interactive_phase_estimation();
                break;
            case 10:
                interactive_shor();
                break;
            case 11:
                printf("Thank you for using the Quantum Computing Simulator!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        
        printf("\nPress Enter to continue...");
        getchar();
    }
    
    return 0;
}
