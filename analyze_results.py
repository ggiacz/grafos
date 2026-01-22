#!/usr/bin/env python3
"""
Script para análise de resultados do L(p,q)-coloring
Gera tabelas de desvio percentual para o relatório
"""

import csv
import sys
from collections import defaultdict
import statistics

def read_results(csv_file):
    """Lê resultados do arquivo CSV"""
    results = []
    try:
        with open(csv_file, 'r') as f:
            reader = csv.DictReader(f)
            for row in reader:
                results.append(row)
        return results
    except FileNotFoundError:
        print(f"Erro: Arquivo {csv_file} não encontrado!")
        sys.exit(1)

def organize_by_algorithm(results):
    """Organiza resultados por algoritmo e instância"""
    data = defaultdict(lambda: defaultdict(list))
    
    for row in results:
        instance = row['instance']
        algorithm = row['algorithm']
        if algorithm == 'randomized':
            algorithm = f"randomized_alpha_{row['alpha']}"
        
        best_solution = int(row['bestSolution'])
        data[instance][algorithm].append(best_solution)
    
    return data

def calculate_deviations(data, best_known):
    """Calcula desvios percentuais"""
    deviations = {}
    
    for instance, algorithms in data.items():
        deviations[instance] = {}
        known = best_known.get(instance, None)
        
        for algorithm, solutions in algorithms.items():
            best_found = min(solutions)
            avg_found = statistics.mean(solutions)
            
            if known:
                dev_best = ((best_found - known) / known) * 100
                dev_avg = ((avg_found - known) / known) * 100
            else:
                dev_best = 0.0
                dev_avg = 0.0
            
            deviations[instance][algorithm] = {
                'best': best_found,
                'avg': avg_found,
                'dev_best': dev_best,
                'dev_avg': dev_avg,
                'num_executions': len(solutions)
            }
    
    return deviations

def calculate_average_times(results):
    """Calcula tempo médio por algoritmo e instância"""
    times = defaultdict(lambda: defaultdict(list))
    
    for row in results:
        instance = row['instance']
        algorithm = row['algorithm']
        if algorithm == 'randomized':
            algorithm = f"randomized_alpha_{row['alpha']}"
        
        exec_time = float(row['executionTime'])
        times[instance][algorithm].append(exec_time)
    
    avg_times = {}
    for instance, algorithms in times.items():
        avg_times[instance] = {}
        for algorithm, time_list in algorithms.items():
            avg_times[instance][algorithm] = statistics.mean(time_list)
    
    return avg_times

def print_latex_table_best_deviation(deviations, instances, algorithms):
    """Imprime tabela de desvio da melhor solução em formato LaTeX"""
    print("\n% Tabela 1: Desvio percentual da melhor solução")
    print("\\begin{table}[h]")
    print("\\centering")
    print("\\caption{Desvio percentual da melhor solução encontrada}")
    print("\\begin{tabular}{|l|" + "c|" * len(algorithms) + "}")
    print("\\hline")
    print("\\textbf{Instância} & " + " & ".join([f"\\textbf{{{alg}}}" for alg in algorithms]) + " \\\\")
    print("\\hline")
    
    for instance in instances:
        row = [instance]
        for algorithm in algorithms:
            if algorithm in deviations[instance]:
                dev = deviations[instance][algorithm]['dev_best']
                row.append(f"{dev:.2f}\\%")
            else:
                row.append("-")
        print(" & ".join(row) + " \\\\")
    
    print("\\hline")
    print("\\end{tabular}")
    print("\\end{table}")

def print_latex_table_avg_deviation(deviations, instances, algorithms):
    """Imprime tabela de desvio da média em formato LaTeX"""
    print("\n% Tabela 2: Desvio percentual da média")
    print("\\begin{table}[h]")
    print("\\centering")
    print("\\caption{Desvio percentual da média das soluções}")
    print("\\begin{tabular}{|l|" + "c|" * len(algorithms) + "}")
    print("\\hline")
    print("\\textbf{Instância} & " + " & ".join([f"\\textbf{{{alg}}}" for alg in algorithms]) + " \\\\")
    print("\\hline")
    
    for instance in instances:
        row = [instance]
        for algorithm in algorithms:
            if algorithm in deviations[instance]:
                dev = deviations[instance][algorithm]['dev_avg']
                row.append(f"{dev:.2f}\\%")
            else:
                row.append("-")
        print(" & ".join(row) + " \\\\")
    
    print("\\hline")
    print("\\end{tabular}")
    print("\\end{table}")

def print_latex_table_times(avg_times, instances, algorithms):
    """Imprime tabela de tempos médios em formato LaTeX"""
    print("\n% Tabela 3: Tempo médio de execução (segundos)")
    print("\\begin{table}[h]")
    print("\\centering")
    print("\\caption{Tempo médio de execução em segundos}")
    print("\\begin{tabular}{|l|" + "c|" * len(algorithms) + "}")
    print("\\hline")
    print("\\textbf{Instância} & " + " & ".join([f"\\textbf{{{alg}}}" for alg in algorithms]) + " \\\\")
    print("\\hline")
    
    for instance in instances:
        row = [instance]
        for algorithm in algorithms:
            if algorithm in avg_times[instance]:
                time = avg_times[instance][algorithm]
                row.append(f"{time:.4f}")
            else:
                row.append("-")
        print(" & ".join(row) + " \\\\")
    
    print("\\hline")
    print("\\end{tabular}")
    print("\\end{table}")

def main():
    if len(sys.argv) < 2:
        print("Uso: python3 analyze_results.py <arquivo_csv>")
        print("Exemplo: python3 analyze_results.py results/results.csv")
        sys.exit(1)
    
    csv_file = sys.argv[1]
    
    print("=" * 60)
    print("Análise de Resultados - L(p,q)-Coloring")
    print("=" * 60)
    
    # Lê resultados
    print(f"\nLendo resultados de: {csv_file}")
    results = read_results(csv_file)
    print(f"Total de execuções: {len(results)}")
    
    # Organiza dados
    data = organize_by_algorithm(results)
    
    # Melhores soluções conhecidas (AJUSTAR CONFORME LITERATURA!)
    # TODO: Preencher com os valores da literatura para cada instância
    best_known = {}
    # Exemplo:
    # best_known['instances/grafo1.col'] = 10
    # best_known['instances/grafo2.col'] = 15
    
    # Calcula desvios
    deviations = calculate_deviations(data, best_known)
    
    # Calcula tempos médios
    avg_times = calculate_average_times(results)
    
    # Lista de instâncias e algoritmos
    instances = sorted(data.keys())
    algorithms_set = set()
    for inst_data in data.values():
        algorithms_set.update(inst_data.keys())
    algorithms = sorted(algorithms_set)
    
    # Imprime resumo
    print("\n" + "=" * 60)
    print("Resumo dos Resultados")
    print("=" * 60)
    for instance in instances:
        print(f"\nInstância: {instance}")
        for algorithm in algorithms:
            if algorithm in deviations[instance]:
                info = deviations[instance][algorithm]
                print(f"  {algorithm}:")
                print(f"    Melhor: {info['best']}")
                print(f"    Média: {info['avg']:.2f}")
                print(f"    Desvio (melhor): {info['dev_best']:.2f}%")
                print(f"    Desvio (média): {info['dev_avg']:.2f}%")
                print(f"    Tempo médio: {avg_times[instance][algorithm]:.4f}s")
    
    # Gera tabelas LaTeX
    print("\n" + "=" * 60)
    print("Tabelas LaTeX para o Relatório")
    print("=" * 60)
    
    print_latex_table_best_deviation(deviations, instances, algorithms)
    print_latex_table_avg_deviation(deviations, instances, algorithms)
    print_latex_table_times(avg_times, instances, algorithms)
    
    print("\n" + "=" * 60)
    print("IMPORTANTE: Ajuste os valores de 'best_known' no script")
    print("com as melhores soluções conhecidas da literatura!")
    print("=" * 60)

if __name__ == "__main__":
    main()
