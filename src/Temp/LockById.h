#pragma once

class LockById {
public:
	explicit LockById(std::string);

	static std::shared_ptr<LockById> getInstance(const std::string&);

	void lock(uint_fast32_t);
	void unlock(uint_fast32_t);

private:
	static std::map<std::string, std::shared_ptr<LockById>, std::less<>> instances;

	std::string instanceName;

	std::mutex accessMutex;
	std::map<uint_fast32_t, std::mutex> mutexById;

};