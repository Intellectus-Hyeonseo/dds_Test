import pandas as pd
import matplotlib.pyplot as plt
import re
import sys
import os
import numpy as np

def parse_frame_stats(file_path):
    data = {
        "FrameIndex": [],
        "IAT": [],
        "Jitter": [],
        "FPS": []
    }

    pattern = re.compile(r"FrameIndex:\s*(\d+),\s*IAT:\s*([\d.]+)\s*ms,\s*Jitter:\s*([\d.]+)\s*ms,\s*FPS:\s*(\d+)")
    
    with open(file_path, "r") as file:
        for line in file:
            match = pattern.search(line)
            if match:
                data["FrameIndex"].append(int(match.group(1)))
                data["IAT"].append(float(match.group(2)))
                data["Jitter"].append(float(match.group(3)))
                data["FPS"].append(int(match.group(4)))
    
    df = pd.DataFrame(data)
    return df

def plot_multiple_files(file_paths):
    if not file_paths:
        print("Usage: python analyzer.py <file1.txt> <file2.txt> ...")
        sys.exit(1)

    plt.figure(figsize=(16, 8))

    # IAT Plot
    plt.subplot(3, 1, 1)
    for path in file_paths:
        if not os.path.exists(path):
            print(f"Warning: File not found - {path}")
            continue
        df = parse_frame_stats(path)
        if df.empty:
            print(f"Warning: No valid data in {path}")
            continue
        plt.plot(np.array(df["FrameIndex"]), np.array(df["IAT"]),
                 label=os.path.basename(path))
    plt.title("IAT (Inter-Arrival Time)")
    plt.ylabel("IAT (ms)")
    plt.grid(True)
    plt.legend()

    # Jitter Plot
    plt.subplot(3, 1, 2)
    for path in file_paths:
        if not os.path.exists(path):
            continue
        df = parse_frame_stats(path)
        if df.empty:
            continue
        plt.plot(np.array(df["FrameIndex"]), np.array(df["Jitter"]),
                 label=os.path.basename(path))
    plt.title("Jitter")
    plt.ylabel("Jitter (ms)")
    plt.grid(True)
    plt.legend()

    # FPS Plot
    plt.subplot(3, 1, 3)
    for path in file_paths:
        if not os.path.exists(path):
            continue
        df = parse_frame_stats(path)
        if df.empty:
            continue
        plt.plot(np.array(df["FrameIndex"]), np.array(df["FPS"]),
                 label=os.path.basename(path))
    plt.title("FPS")
    plt.xlabel("Frame Index")
    plt.ylabel("FPS")
    plt.grid(True)
    plt.legend()

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    file_paths = sys.argv[1:]
    plot_multiple_files(file_paths)
