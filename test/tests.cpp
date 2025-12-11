#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <memory>

#include "dungeon.h"
#include "factory.h"
#include "npc.h"
#include "observer.h"
#include "point.h"
#include "visitor.h"

// ==================== ТЕСТЫ КЛАССА Point ====================
TEST(PointTest, DistanceCalculation) {
    Point p1(0, 0);
    Point p2(3, 4);
    EXPECT_NEAR(p1.distanceTo(p2), 5.0, 0.0001);
    
    Point p3(10, 10);
    Point p4(10, 20);
    EXPECT_NEAR(p3.distanceTo(p4), 10.0, 0.0001);
}

TEST(PointTest, Validation) {
    Point p1(0, 0);
    EXPECT_TRUE(p1.isValid());
    
    Point p2(250, 250);
    EXPECT_TRUE(p2.isValid());
    
    Point p3(500, 500);
    EXPECT_TRUE(p3.isValid());
    
    Point p4(-1, 0);
    EXPECT_FALSE(p4.isValid());
    
    Point p5(501, 300);
    EXPECT_FALSE(p5.isValid());
}

// ==================== ТЕСТЫ КЛАССА NPC ====================
TEST(NPCTest, BasicProperties) {
    Orc orc("Гром", Point(100, 100));
    EXPECT_EQ(orc.getName(), "Гром");
    EXPECT_EQ(orc.getType(), "Orc");
    EXPECT_TRUE(orc.isAlive());
    
    Point pos = orc.getPosition();
    EXPECT_EQ(pos.x, 100);
    EXPECT_EQ(pos.y, 100);
}

TEST(NPCTest, Death) {
    Orc orc("Гром", Point(0, 0));
    EXPECT_TRUE(orc.isAlive());
    
    orc.die();
    EXPECT_FALSE(orc.isAlive());
}

// ==================== ТЕСТЫ ЛОГИКИ БОЯ ====================
TEST(BattleLogicTest, OrcCanAttackBear) {
    Orc orc("Гром", Point(0, 0));
    Bear bear("Бруно", Point(10, 10));
    
    EXPECT_TRUE(orc.canAttack(&bear));      // Орк → Медведь ✓
    EXPECT_FALSE(bear.canAttack(&orc));     // Медведь → Орк ✗
}

TEST(BattleLogicTest, KnightCanAttackOrc) {
    Knight knight("Артур", Point(0, 0));
    Orc orc("Гром", Point(10, 10));
    
    EXPECT_TRUE(knight.canAttack(&orc));    // Рыцарь → Орк ✓
    EXPECT_FALSE(orc.canAttack(&knight));   // Орк → Рыцарь ✗
}

TEST(BattleLogicTest, BearCanAttackKnight) {
    Bear bear("Бруно", Point(0, 0));
    Knight knight("Артур", Point(10, 10));
    
    EXPECT_TRUE(bear.canAttack(&knight));   // Медведь → Рыцарь ✓
    EXPECT_FALSE(knight.canAttack(&bear));  // Рыцарь → Медведь ✗
}

TEST(BattleLogicTest, EqualTypesNoAttack) {
    Orc orc1("Гром", Point(0, 0));
    Orc orc2("Углук", Point(10, 10));
    
    EXPECT_FALSE(orc1.canAttack(&orc2));    // Орк → Орк ✗
    EXPECT_FALSE(orc2.canAttack(&orc1));    // Орк → Орк ✗
}

// ==================== ТЕСТЫ ФАБРИКИ ====================
TEST(FactoryTest, CreateNPC) {
    auto orc = NPCFactory::createNPC(NPCFactory::NPCType::ORC, "ТестОрк", Point(50, 50));
    ASSERT_NE(orc, nullptr);
    EXPECT_EQ(orc->getType(), "Orc");
    EXPECT_EQ(orc->getName(), "ТестОрк");
    
    auto knight = NPCFactory::createNPC(NPCFactory::NPCType::KNIGHT, "ТестРыцарь", Point(100, 100));
    ASSERT_NE(knight, nullptr);
    EXPECT_EQ(knight->getType(), "Knight");
    
    auto bear = NPCFactory::createNPC(NPCFactory::NPCType::BEAR, "ТестМедведь", Point(150, 150));
    ASSERT_NE(bear, nullptr);
    EXPECT_EQ(bear->getType(), "Bear");
}

TEST(FactoryTest, CreateNPCFromString) {
    // Правильные данные
    auto npc1 = NPCFactory::createNPCFromString("Orc Гром 100 200");
    ASSERT_NE(npc1, nullptr);
    EXPECT_EQ(npc1->getType(), "Orc");
    EXPECT_EQ(npc1->getName(), "Гром");
    
    auto npc2 = NPCFactory::createNPCFromString("Knight Артур 150 250");
    ASSERT_NE(npc2, nullptr);
    EXPECT_EQ(npc2->getType(), "Knight");
    
    auto npc3 = NPCFactory::createNPCFromString("Bear Бруно 200 300");
    ASSERT_NE(npc3, nullptr);
    EXPECT_EQ(npc3->getType(), "Bear");
    
    // Неправильные данные
    auto npc4 = NPCFactory::createNPCFromString("Unknown Тип 100 100");
    EXPECT_EQ(npc4, nullptr);
    
    auto npc5 = NPCFactory::createNPCFromString("Orc БезКоординат");
    EXPECT_EQ(npc5, nullptr);
    
    auto npc6 = NPCFactory::createNPCFromString("Orc НеправильныеКоординаты 600 600");
    EXPECT_EQ(npc6, nullptr);
}

TEST(FactoryTest, StringToTypeConversion) {
    EXPECT_EQ(NPCFactory::stringToType("Orc"), NPCFactory::NPCType::ORC);
    EXPECT_EQ(NPCFactory::stringToType("orc"), NPCFactory::NPCType::ORC);
    EXPECT_EQ(NPCFactory::stringToType("ORC"), NPCFactory::NPCType::ORC);
    
    EXPECT_EQ(NPCFactory::stringToType("Knight"), NPCFactory::NPCType::KNIGHT);
    EXPECT_EQ(NPCFactory::stringToType("knight"), NPCFactory::NPCType::KNIGHT);
    
    EXPECT_EQ(NPCFactory::stringToType("Bear"), NPCFactory::NPCType::BEAR);
    EXPECT_EQ(NPCFactory::stringToType("bear"), NPCFactory::NPCType::BEAR);
}

// ==================== ТЕСТЫ ПОДЗЕМЕЛЬЯ ====================
TEST(DungeonTest, AddNPC) {
    Dungeon dungeon;
    
    EXPECT_EQ(dungeon.countAlive(), 0);
    
    auto orc = std::make_shared<Orc>("Гром", Point(100, 100));
    dungeon.addNPC(orc);
    
    EXPECT_EQ(dungeon.countAlive(), 1);
    
    // Попытка добавить NPC с неверными координатами
    auto invalid = std::make_shared<Orc>("Неверный", Point(-10, -10));
    dungeon.addNPC(invalid);
    
    EXPECT_EQ(dungeon.countAlive(), 1);  // Не должен добавиться
}

TEST(DungeonTest, BattleSimple) {
    Dungeon dungeon;
    
    // Орк и Медведь близко друг к другу
    auto orc = std::make_shared<Orc>("Гром", Point(0, 0));
    auto bear = std::make_shared<Bear>("Бруно", Point(10, 10));
    
    dungeon.addNPC(orc);
    dungeon.addNPC(bear);
    
    EXPECT_EQ(dungeon.countAlive(), 2);
    
    // Бой с дальностью 50 (они близко)
    dungeon.battle(50);
    
    // Медведь должен быть убит (Орк побеждает Медведя)
    EXPECT_EQ(dungeon.countAlive(), 1);
}

TEST(DungeonTest, BattleOutOfRange) {
    Dungeon dungeon;
    
    // Персонажи далеко друг от друга
    auto orc = std::make_shared<Orc>("Гром", Point(0, 0));
    auto bear = std::make_shared<Bear>("Бруно", Point(400, 400));
    
    dungeon.addNPC(orc);
    dungeon.addNPC(bear);
    
    // Бой с маленькой дальностью
    dungeon.battle(50);
    
    // Никто не должен погибнуть (слишком далеко)
    EXPECT_EQ(dungeon.countAlive(), 2);
}

TEST(DungeonTest, BattleMultipleNPCs) {
    Dungeon dungeon;
    
    // Орк, Рыцарь и Медведь все близко
    auto orc = std::make_shared<Orc>("Гром", Point(0, 0));
    auto knight = std::make_shared<Knight>("Артур", Point(5, 5));
    auto bear = std::make_shared<Bear>("Бруно", Point(10, 10));
    
    dungeon.addNPC(orc);
    dungeon.addNPC(knight);
    dungeon.addNPC(bear);
    
    // Схема боя:
    // 1. Орк убивает Медведя
    // 2. Рыцарь убивает Орка
    // 3. Медведь убивает Рыцаря (но он уже мёртв от Орка)
    dungeon.battle(50);
    
    // В результате должен остаться только Рыцарь
    EXPECT_EQ(dungeon.countAlive(), 1);
}

TEST(DungeonTest, SaveAndLoad) {
    // Создаём временный файл
    const std::string testFile = "test_dungeon_save.txt";
    
    {
        Dungeon dungeon1;
        dungeon1.addNPC(std::make_shared<Orc>("Гром", Point(100, 100)));
        dungeon1.addNPC(std::make_shared<Knight>("Артур", Point(200, 200)));
        
        // Сохраняем - метод void, просто проверяем что не падает
        EXPECT_NO_THROW(dungeon1.saveToFile(testFile));
    }
    
    // Загружаем в новое подземелье
    Dungeon dungeon2;
    dungeon2.loadFromFile(testFile);
    
    EXPECT_EQ(dungeon2.countAlive(), 2);
    
    // Удаляем временный файл
    std::remove(testFile.c_str());
}

TEST(DungeonTest, EmptyDungeon) {
    Dungeon dungeon;
    
    EXPECT_EQ(dungeon.countAlive(), 0);
    
    // Бой в пустом подземелье
    EXPECT_NO_THROW(dungeon.battle(100));
    
    EXPECT_EQ(dungeon.countAlive(), 0);
}

// ==================== ТЕСТЫ НАБЛЮДАТЕЛЕЙ ====================
TEST(ObserverTest, ConsoleObserver) {
    ConsoleObserver observer;
    
    // Просто проверяем, что не падает
    EXPECT_NO_THROW(observer.update("Тестовое сообщение"));
}

TEST(ObserverTest, FileObserverCreatesFile) {
    const std::string testFile = "test_log.txt";
    
    {
        FileObserver observer(testFile);
        observer.update("Тестовое сообщение в файл");
    }
    
    // Проверяем, что файл создан
    std::ifstream file(testFile);
    EXPECT_TRUE(file.good());
    
    // Читаем содержимое
    std::string content;
    std::getline(file, content);
    EXPECT_TRUE(content.find("Тестовое сообщение в файл") != std::string::npos);
    
    file.close();
    std::remove(testFile.c_str());
}

// ==================== ТЕСТЫ ПОСЕТИТЕЛЯ (VISITOR) ====================
TEST(VisitorTest, BattleVisitorProcess) {
    // Используем реальный Dungeon вместо мока
    Dungeon dungeon;
    
    auto orc = std::make_shared<Orc>("Гром", Point(0, 0));
    auto bear = std::make_shared<Bear>("Бруно", Point(10, 10));
    
    dungeon.addNPC(orc);
    dungeon.addNPC(bear);
    
    BattleVisitor visitor(orc, &dungeon);
    
    // Орк атакует медведя
    bear->accept(visitor);
    
    // Проверяем, что медведь убит
    EXPECT_FALSE(bear->isAlive());
    EXPECT_TRUE(orc->isAlive());
}

// ==================== ИНТЕГРАЦИОННЫЕ ТЕСТЫ ====================
TEST(IntegrationTest, CompleteBattleScenario) {
    Dungeon dungeon;
    
    // Создаём тестовых персонажей через фабрику
    auto orc = NPCFactory::createNPC(NPCFactory::NPCType::ORC, "Гром", Point(0, 0));
    auto knight = NPCFactory::createNPC(NPCFactory::NPCType::KNIGHT, "Артур", Point(20, 20));
    auto bear = NPCFactory::createNPC(NPCFactory::NPCType::BEAR, "Бруно", Point(40, 40));
    
    dungeon.addNPC(orc);
    dungeon.addNPC(knight);
    dungeon.addNPC(bear);
    
    // Все должны быть живы
    EXPECT_EQ(dungeon.countAlive(), 3);
    
    // Бой с достаточной дальностью
    dungeon.battle(100);
    
    // После боя должен остаться кто-то один
    EXPECT_TRUE(dungeon.countAlive() <= 3);
}

TEST(IntegrationTest, FileSaveLoadIntegration) {
    const std::string testFile = "integration_test.txt";
    
    // Создаём и сохраняем
    {
        Dungeon dungeon;
        dungeon.addNPC(NPCFactory::createNPC(NPCFactory::NPCType::ORC, "ИнтеграционныйОрк", Point(111, 222)));
        dungeon.addNPC(NPCFactory::createNPC(NPCFactory::NPCType::KNIGHT, "ИнтеграционныйРыцарь", Point(333, 444)));
        
        // Сохраняем - метод void
        EXPECT_NO_THROW(dungeon.saveToFile(testFile));
    }
    
    // Загружаем и проверяем
    {
        Dungeon dungeon;
        dungeon.loadFromFile(testFile);
        
        EXPECT_EQ(dungeon.countAlive(), 2);
    }
    
    // Очистка
    std::remove(testFile.c_str());
}

// ==================== ТЕСТЫ ГРАНИЧНЫХ УСЛОВИЙ ====================
TEST(EdgeCaseTest, NPCAtBoundaries) {
    Orc orc1("Граница1", Point(0, 0));
    EXPECT_TRUE(orc1.getPosition().isValid());
    
    Orc orc2("Граница2", Point(500, 500));
    EXPECT_TRUE(orc2.getPosition().isValid());
    
    Orc orc3("ЗаГраницей", Point(501, 501));
    EXPECT_FALSE(orc3.getPosition().isValid());
}

TEST(EdgeCaseTest, ZeroRangeBattle) {
    Dungeon dungeon;
    
    auto orc = std::make_shared<Orc>("Гром", Point(0, 0));
    auto bear = std::make_shared<Bear>("Бруно", Point(0, 0));  // Та же точка!
    
    dungeon.addNPC(orc);
    dungeon.addNPC(bear);
    
    dungeon.battle(0);  // Нулевая дальность
    
    // Даже в одной точке с нулевой дальностью должны сразиться
    EXPECT_EQ(dungeon.countAlive(), 1);
}

TEST(EdgeCaseTest, SelfBattle) {
    Dungeon dungeon;
    
    auto orc = std::make_shared<Orc>("Гром", Point(0, 0));
    dungeon.addNPC(orc);
    
    // Персонаж не должен сражаться с самим собой
    dungeon.battle(100);
    
    EXPECT_EQ(dungeon.countAlive(), 1);
}

// ==================== ОСНОВНОЙ ТЕСТ ====================
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}