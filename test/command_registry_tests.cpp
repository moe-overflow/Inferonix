#include <gtest/gtest.h>
#include <util/command_registry.hpp>
#include <util/logger.hpp>

#include <string>
#include <vector>

using inferonix::utils::command_registry;

TEST(CommandRegistry, ExecutesRegisteredCommand)
{
    command_registry registry;
    int calls = 0;

    registry.register_command("hello", "greet", [&](auto const&) { ++calls; });
    registry.execute("hello");

    EXPECT_EQ(calls, 1);
}

TEST(CommandRegistry, PassesWhitespaceSplitArguments)
{
    command_registry registry;
    std::vector<std::string> received;

    registry.register_command("dump", "dump args", [&](auto const& args) { received = args; });
    registry.execute("dump one two   three");

    ASSERT_EQ(received.size(), 3u);
    EXPECT_EQ(received[0], "one");
    EXPECT_EQ(received[1], "two");
    EXPECT_EQ(received[2], "three");
}

TEST(CommandRegistry, ExecutesWithoutArguments)
{
    command_registry registry;
    std::size_t args_count = 42;

    registry.register_command("noop", "no args", [&](auto const& args) { args_count = args.size(); });
    registry.execute("noop");

    EXPECT_EQ(args_count, 0u);
}

TEST(CommandRegistry, HandleCommandDispatchesDirectly)
{
    command_registry registry;
    std::vector<std::string> received;

    registry.register_command("sum", "sum", [&](auto const& args) { received = args; });
    registry.handle_command("sum", { "1", "2", "3" });

    ASSERT_EQ(received.size(), 3u);
    EXPECT_EQ(received[0], "1");
    EXPECT_EQ(received[2], "3");
}

TEST(CommandRegistry, EmptyCommandIsNoOp)
{
    command_registry registry;
    int calls = 0;

    registry.register_command("hello", "greet", [&](auto const&) { ++calls; });
    registry.execute("");

    EXPECT_EQ(calls, 0);
}

TEST(CommandRegistry, UnknownCommandDoesNotCrash)
{
    command_registry registry;
    registry.execute("no_such_command");
    SUCCEED();
}

TEST(CommandRegistry, OverwritesPreviouslyRegisteredCommand)
{
    command_registry registry;
    std::string result = "initial";

    registry.register_command("cmd", "first", [&](auto const&) { result = "first"; });
    registry.register_command("cmd", "second", [&](auto const&) { result = "second"; });
    registry.execute("cmd");

    EXPECT_EQ(result, "second");
}
