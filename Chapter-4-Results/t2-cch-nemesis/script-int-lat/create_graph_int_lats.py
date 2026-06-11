import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

# ── Input data ────────────────────────────────────────────────────────────────
# Set to an integer to limit how many trace elements are plotted.
TRACE_ELEMENTS_TO_PLOT = 20

trace_file = Path(__file__).resolve().parent.parent / "logs" / "trace_test.txt"
trace_values = [int(x) for x in trace_file.read_text(encoding="utf-8").split()]
if TRACE_ELEMENTS_TO_PLOT is not None:
    trace_values = trace_values[:TRACE_ELEMENTS_TO_PLOT]

encoding = {
    1: ("MOV", 48),
    2: ("LDR", 49),
}

data = []
for value in trace_values:
    if value not in encoding:
        raise ValueError(f"Unsupported trace value in {trace_file}: {value}")
    data.append(encoding[value])

# ── Series ────────────────────────────────────────────────────────────────────
labels = [inst for inst, _ in data]
latencies = np.array([v for _, v in data])

# We use intervals [0, 1], [1, 2], etc. for each instruction
x_starts = np.arange(len(data))

# ── Style ─────────────────────────────────────────────────────────────────────
plt.rcParams.update({
    # "font.family":    "serif",
    "font.serif":     ["Times New Roman", "DejaVu Serif"],
    "font.size":      12,
    "axes.labelsize": 13,
    "figure.dpi":     150,
    # "font.weight":    "bold",
    # "axes.labelweight": "bold",
    # "axes.titleweight": "bold",
})


# ── Figure ────────────────────────────────────────────────────────────────────
fig, ax = plt.subplots(figsize=(14, 2.6))

# To make the square wave stretch across the final interval, 
# we append the final coordinates to our arrays.
x_step = np.append(x_starts, len(data))
y_step = np.append(latencies, latencies[-1])

# 1. Plot the "Square Wave" (Step plot)
ax.step(x_step, y_step, where='post', color="#b75700", linewidth=4, zorder=3)

# 2. Add the vertical division lines (orange)
# We draw them at the boundaries: x = 1, 2, 3...
for x_val in range(1, len(data)):
    ax.axvline(x=x_val, color="orange", linewidth=1, linestyle="--", zorder=1)

# ── Spines & Arrows ───────────────────────────────────────────────────────────
for spine in ["top", "right"]:
    ax.spines[spine].set_visible(False)
for spine in ["left", "bottom"]:
    ax.spines[spine].set_linewidth(1.2)

# Arrow-tipped axes using reliable transAxes coordinates
ax.plot(1, 0, ">k", transform=ax.transAxes, clip_on=False, markersize=5)
ax.plot(0, 1, "^k", transform=ax.transAxes, clip_on=False, markersize=5)

# ── Axes & labels ─────────────────────────────────────────────────────────────
ax.set_ylabel("IRQ Latency", labelpad=6, rotation=90, va="center")

# Y-axis
lat_min, lat_max = latencies.min(), latencies.max()
y_ticks = np.arange(lat_min - 1, lat_max + 2, 1)
ax.set_yticks(y_ticks)
ax.set_ylim(lat_min - 1.5, lat_max + 1.5)

# X-axis
ax.set_xlim(0, len(data))

# 3. Center the labels on the x-axis for each interval
ax.set_xticks(x_starts + 0.5)  # Shift ticks to the middle of the intervals
ax.set_xticklabels(labels)
ax.tick_params(axis='x', length=0) # Hide the actual tick marks for a cleaner look

# No grid
ax.yaxis.grid(False)
ax.xaxis.grid(False)

ax.set_facecolor("#fff7ed")


# ── Save / show ───────────────────────────────────────────────────────────────
plt.tight_layout()
base_dir = Path(__file__+"/..").resolve().parent
output_dir = base_dir / "outputs"
out_file_pdf = output_dir / "irq_latency_mov_ldr.pdf"
out_file_png = output_dir / "irq_latency_mov_ldr.png"
plt.savefig(out_file_pdf, bbox_inches="tight")
plt.savefig(out_file_png, bbox_inches="tight")
# plt.show()