package main

import (
	"fmt"
	"math"
	"os"
	"os/signal"
	"time"
)

type path struct {
	dest int
	cost int
}

func findMin(dist []int, visited []bool) (int, []bool) {
	min, idx := math.MaxInt32, 0
	for i, e := range dist {
		if visited[i] {
			continue
		}
		if e < min {
			min = e
			idx = i
		}
	}
	return idx, visited
}

func dijkstraj(graph [][]path, vertex int) []int {
	distance := make([]int, vertex)
	visited := make([]bool, vertex)
	route := make([]int, 1)
	// start from 0, so point 0 distance = 0
	for i := 1; i < len(distance); i++ {
		distance[i] = math.MaxInt32
	}

	currVertex := 0
	visited[currVertex] = true
	for i := 0; i < vertex-1; i++ {
		for _, edge := range graph[currVertex] {
			if edge.cost+distance[currVertex] < distance[edge.dest] {
				distance[edge.dest] = edge.cost + distance[currVertex]
			}
		}
		currVertex, visited = findMin(distance, visited)
		visited[currVertex] = true
		route = append(route, currVertex)
	}
	return route
}

func main() {
	ch := make(chan os.Signal)
	signal.Notify(ch, os.Interrupt)
	go func() {
		<-ch
		fmt.Println("SIGINT")
		time.Sleep(time.Second * 1)
		os.Exit(-1)
	}()
	graph := [][]path{
		{path{dest: 1, cost: 1}, path{dest: 2, cost: 12}},
		{path{dest: 2, cost: 9}, path{dest: 3, cost: 3}},
		{path{dest: 4, cost: 5}},
		{path{dest: 2, cost: 4}, path{dest: 4, cost: 13}, path{dest: 5, cost: 15}},
		{path{dest: 5, cost: 4}},
	}
	for _, i := range dijkstraj(graph, 6) {
		fmt.Printf("%d ", i)
	}
	fmt.Println()
}
