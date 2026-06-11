#!/usr/bin/env python3
from pathlib import Path

LABELS = [
    "Nemesis Covert-Channel Test",
    "Nemesis Covert-Channel Transmit 0x0f",
    "Nemesis Covert-Channel Transmit 0x55",
    "Nemesis Covert-Channel Transmit 0xf0",
    "Nemesis Covert-Channel Transmit 0xff",
]

OUTPUT_NAMES = {
    "Nemesis Covert-Channel Test": "trace_test.txt",
    "Nemesis Covert-Channel Transmit 0x0f": "C04-cch_0x0f.txt",
    "Nemesis Covert-Channel Transmit 0x55": "C04-cch_0x55.txt",
    "Nemesis Covert-Channel Transmit 0xf0": "C04-cch_0xf0.txt",
    "Nemesis Covert-Channel Transmit 0xff": "C04-cch_0xff.txt",
}

HEADER = (
    "Color_type: Integer #Integer or Probability\n"
    "Gradient: orange\n"
    "Min_number_of_colors: 3\n"
    "Y: bit0 bit1 bit2 bit3 bit4 bit5 bit6 bit7\n"
    "X: It1 It2 It3 It4 It5 It6 It7 It8"
)


def parse_trace_line(line):
    return [int(token) for token in line.strip().split()]

# Put the vector in a matrix:
# For example, a vector like:
# 1 1 1 1 2 2 2 2
# would be represented as:
# 0 0 0 0 0 0 0 2
# 0 0 0 0 0 0 2 0
# 0 0 0 0 0 2 0 0
# 0 0 0 0 2 0 0 0
# 0 0 0 1 0 0 0 0
# 0 0 1 0 0 0 0 0
# 0 1 0 0 0 0 0 0
# 1 0 0 0 0 0 0 0
def format_trace_as_matrix(trace):
    n = len(trace)
    rows = []
    for row in range(n):
        values = [0] * n
        col = n - 1 - row
        values[col] = trace[col]
        rows.append(" ".join(str(x) for x in values) + " ")
    return "\n".join(rows)


def main():
    base_dir = Path(__file__).resolve().parent
    input_file = base_dir / "logs" / "0_raw_trace.txt"
    output_dir = base_dir / "logs"
    output_dir.mkdir(parents=True, exist_ok=True)

    lines = input_file.read_text(encoding="utf-8").splitlines()

    label_to_trace = {}
    for i, line in enumerate(lines):
        text = line.strip()
        if text in LABELS:
            if i + 1 >= len(lines):
                raise ValueError(f"Missing trace line after label: {text}")
            trace = parse_trace_line(lines[i + 1])
            label_to_trace[text] = trace

    # 2D buffer with 5 vectors, one per trace label (in LABELS order).
    traces_2d = []
    for label in LABELS:
        if label not in label_to_trace:
            raise ValueError(f"Label not found in input: {label}")
        traces_2d.append(label_to_trace[label])
        
    # Pick the elements in traces_2d 1 to 4, remove the first element (shift left), 
    # and keep only the elements 1 to 8 (inclusive) of the resulting vector.
    for i in range(0, 5):
        trace = traces_2d[i]
        shifted_trace = trace[1:]  # Remove the first element (shift left)
        trace_size = len(shifted_trace)
        # Remove the last 6 elements, it is trace from scaffold code and not relevant for the attack
        truncated_trace = shifted_trace[:(trace_size-6)]  
        traces_2d[i] = truncated_trace
        
    # Write the first trace (corresponding to Int lat test")
    out_file = output_dir / OUTPUT_NAMES[LABELS[0]]
    out_file.write_text(" ".join(str(x) for x in traces_2d[0]) + "\n", encoding="utf-8")
        
    for i in range(1, 5):
        out_file = output_dir / OUTPUT_NAMES[LABELS[i]]
        out_file.write_text(
            HEADER + "\n\n" + format_trace_as_matrix(traces_2d[i]) + "\n", encoding="utf-8",
        )

if __name__ == "__main__":
    main()
