#pragma once

class NetworkProfiler
{
public:
    NetworkProfiler(/* args */);
    void Benchmark(int client_socket_fd);
    static void BenchmarkLatency(int client_socket_fd);
    static void BenchmarkThroughput(int client_socket_fd);

private:
};
