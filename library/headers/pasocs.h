#pragma once

#include <vector>
#include <string>
#include "pasocslib_export.h"

#include <torch/torch.h>

namespace wrapper {
	class Wrapper;
}

namespace pasocs {
	class PASOCSLIB_EXPORT Pasocs {
		public:

			Pasocs(int cpus = 1, int verbose = 0, int owa_method = 0);
			//Wrapper(std::ifstream& file, int cpus = 1, int verbose = 0, std::vector<syntax::RawQuery> queries_str = std::vector<syntax::RawQuery>());
			Pasocs(const char* program, int cpus = 1, int verbose = 0, int owa_method = 0);
			~Pasocs();

			void ComputeProbabilities(int mode, int min_samples, int max_samples, int burn, double p_change_hm, int block_gibbs, double percentile, double threshold, int sample_batch);

			std::tuple<torch::Tensor, torch::Tensor, torch::Tensor> ComputeProbabilitiesGraph(std::vector<torch::Tensor> input_anns = std::vector<torch::Tensor>{}, torch::Tensor index_examples = torch::Tensor{});

			void LearnProbabilities(int mode, int min_samples, int max_samples, double percentile, double threshold, int sample_batch, int iterations, double jp_threshold);
			std::string PrintWeightLearningResult();

			void To(torch::Device device);

			void LoadOWA(std::string& path);
			void PrintOWA();
			void DumpOWA(std::string& path);
			void ComputeOWAs(std::vector<double> values, std::vector<int> worlds);

			std::string GetParsedProgram();
			std::vector<std::pair<double, std::string>> GetProbFacts(bool print = false);

			std::vector<std::vector<std::pair<double, std::string>>> GetAnnotatedDisj(bool print = false);
			std::string GetQueries();

			std::string GetExampleSet();

			std::vector<std::string> SampleAnswerSets(int n, double optimism);

			void ComputeGraph();
			torch::Tensor ForwardPoint(std::vector<torch::Tensor> input_anns = std::vector<torch::Tensor>{}, torch::Tensor index_examples = torch::Tensor{});

			torch::Tensor ForwardPointGetAllProbs(std::vector<torch::Tensor> input_anns = std::vector<torch::Tensor>{});
			std::tuple<torch::Tensor, torch::Tensor, torch::Tensor> ForwardBoth(std::vector<torch::Tensor> input_anns = std::vector<torch::Tensor>{}, torch::Tensor index_examples = torch::Tensor{});

			torch::Device GetDevice();

			bool SerializeToFile(const char* path);

			bool DeserializeFromFile(const char* path);

			bool IsGraphComputed();

		private:
			wrapper::Wrapper* wrapper_;
	};
}