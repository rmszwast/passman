#include "yamlex.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

// Class representing an individual entry
class Entry
{
  private:
    std::string name;
    std::string username;
    std::string password;
    std::string url;
    std::string notes;
    std::vector<std::string> groups;

  public:
    // Constructor with initialization list
    Entry(const std::string& name,
          const std::string& username,
          const std::string& password,
          const std::string& url,
          const std::string& notes,
          const std::vector<std::string>& groups)
      : name(name)
      , username(username)
      , password(password)
      , url(url)
      , notes(notes)
      , groups(groups)
    {
    }

    // Default constructor
    Entry() = default;

    // Serialize Entry to YAML
    YAML::Node toYAML() const
    {
        YAML::Node node;
        node["name"] = name;
        node["username"] = username;
        node["password"] = password;
        node["url"] = url;
        node["notes"] = notes;
        node["groups"] = groups;
        return node;
    }

    // Deserialize Entry from YAML
    void fromYAML(const YAML::Node& node)
    {
        name = node["name"].as<std::string>();
        username = node["username"].as<std::string>();
        password = node["password"].as<std::string>();
        url = node["url"].as<std::string>();
        notes = node["notes"].as<std::string>();
        groups = node["groups"].as<std::vector<std::string>>();
    }

    // Display function
    void display() const
    {
        std::cout << "Name: " << name << "\nUsername: " << username
                  << "\nPassword: " << password << "\nURL: " << url
                  << "\nNotes: " << notes << "\nGroups: ";
        for (const auto& group : groups) {
            std::cout << group << " ";
        }
        std::cout << "\n";
    }
};

// Class representing a vault containing multiple entries
class Vault
{
  private:
    std::string name;
    std::vector<Entry> entries;

  public:
    // Constructor
    Vault(const std::string& name)
      : name(name)
    {
    }

    // Default constructor
    Vault() = default;

    // Serialize Vault to YAML
    YAML::Node toYAML() const
    {
        YAML::Node node;
        node["vault_name"] = name;
        for (const auto& entry : entries) {
            node["entries"].push_back(entry.toYAML());
        }
        return node;
    }

    // Deserialize Vault from YAML
    void fromYAML(const YAML::Node& node)
    {
        name = node["vault_name"].as<std::string>();
        entries.clear();
        for (const auto& entryNode : node["entries"]) {
            Entry entry;
            entry.fromYAML(entryNode);
            entries.push_back(entry);
        }
    }

    // Function to add an entry to the vault
    void addEntry(const Entry& entry) { entries.push_back(entry); }

    // Function to display the vault's details
    void display() const
    {
        std::cout << "Vault Name: " << name << "\nEntries:\n";
        for (const auto& entry : entries) {
            entry.display();
            std::cout << "-------------------\n";
        }
    }
};

// Function to save the vault to a YAML file
void
saveVaultToYAML(const Vault& vault)
{
    std::ofstream file("vault.yaml");
    if (file) {
        YAML::Node node = vault.toYAML();
        file << node;
    }
}

// Function to load a vault from a YAML file
Vault
loadVaultFromYAML()
{
    Vault vault;
    YAML::Node node = YAML::LoadFile("vault.yaml");
    vault.fromYAML(node);
    return vault;
}

// Function to create a vault, save it to YAML, and load it back
void
createAndLoadVault()
{
    // Create some example entries
    Entry entry1("Google Account",
                 "user1",
                 "pass1",
                 "https://google.com",
                 "Primary account",
                 { "Personal", "Work" });
    Entry entry2("GitHub Account",
                 "user2",
                 "pass2",
                 "https://github.com",
                 "Development account",
                 { "Work" });

    // Create a vault and add entries
    Vault myVault("My Vault");
    myVault.addEntry(entry1);
    myVault.addEntry(entry2);

    // Save vault to YAML file
    saveVaultToYAML(myVault);

    // Load a different vault from the same YAML file
    Vault loadedVault = loadVaultFromYAML();

    // Display loaded vault details
    std::cout << "Loaded Vault:\n";
    loadedVault.display();
}
