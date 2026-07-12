#ifndef LIBNET_NETWORK_HPP
#define LIBNET_NETWORK_HPP

#include <string>
#include <vector>

namespace libnet {

    /**
     * @brief Represents a network interface adapter with its MAC and IP addresses.
     */
    struct Adapter {
        std::string mac;
        std::vector<std::string> ips;
    };

    /**
     * @brief Network utility class providing cross-platform network operations.
     */
    class Network {
    public:
        Network() = default;
        ~Network() = default;

         /**
          * @brief Retrieves a list of network adapters with their MAC and IP addresses.
          * @return A vector of Adapter structs.
          */
         static std::vector<Adapter> getAdapters();

         /**
          * @brief Retrieves the local IP addresses of the machine.
          * @return A vector of strings containing all local IP addresses found.
          */
         static std::vector<std::string> getLocalIpAddresses();

         /**
          * @brief Resolves a hostname to its corresponding IP addresses (IPv4 and IPv6).
          * @param host The hostname to resolve.
          * @return A vector of resolved IP address strings.
          */
         static std::vector<std::string> resolveHost(const std::string& host);

        /**
         * @brief Checks if a specific host is reachable (Ping).
         * @param host The hostname or IP address to ping.
         * @return True if the host is reachable, false otherwise.
         */
        static bool ping(const std::string& host);

         /**
          * @brief Validates if a string is a valid IPv4 address.
          * @param ip The IP string to validate.
          * @return True if valid, false otherwise.
          */
         static bool isValidIPv4(const std::string& ip);

         /**
          * @brief Validates if a string is a valid IP address (IPv4 or IPv6).
          * @param ip The IP string to validate.
          * @return True if valid, false otherwise.
          */
         static bool isValidIp(const std::string& ip);
    };

} // namespace libnet

#endif // LIBNET_NETWORK_HPP