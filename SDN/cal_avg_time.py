import os
import statistics

def calculate_average(file_path, prune_percentage=10):
    with open(file_path, 'r') as file:
        data = [list(map(float, line.split())) for line in file.readlines()]

    # Transpose the data to work with columns
    transposed_data = list(map(list, zip(*data)))

    pruned_data = []
    for column in transposed_data:
        column_mean = statistics.mean(column)
        prune_threshold = column_mean * (prune_percentage / 100.0)
        pruned_column = [value for value in column if abs(value - column_mean) < prune_threshold]
        pruned_data.append(pruned_column)

    num_columns = len(pruned_data)
    column_sums = [sum(column) for column in pruned_data]
    #column_avgs = [s / len(pruned_data) for s in column_sums]
    column_avgs = [sum / len(column) for sum, column in zip(column_sums, pruned_data)]
    return column_avgs

def main():
    file_names = [
        "bellman_time_500.txt",
        "bellman_time_100.txt",
        "bellman_time_20.txt",
        "dijkstra_time_500.txt",
        "dijkstra_time_100.txt",
        "dijkstra_time_20.txt",
    ]

    output_file = "./time_data/output_avg_pruned_data.txt"

    with open(output_file, 'w') as output:
        for file_name in file_names:
            file_name = "./time_data/" + file_name
            file_path = os.path.join(os.getcwd(), file_name)
            avg_values = calculate_average(file_path, prune_percentage=50)
            output.write(f"{file_name} {' '.join(map(str, avg_values))}\n")

    print(f"Averages calculated and percentage-pruned data saved to {output_file}")

if __name__ == "__main__":
    main()

