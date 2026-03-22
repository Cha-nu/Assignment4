#include <iostream>
#include <vector>
#include <string>
#include <map>

class PotionRecipe {
public:
    std::string potionName;
    std::vector<std::string> ingredients;

    PotionRecipe(const std::string& name, const std::vector<std::string>& ingredients)
        : potionName(name), ingredients(ingredients) {
    }
};

class StockManager {
private:
    std::map<std::string, int> stock;
    const int MAX_STOCK = 3;

public:
    // 초기 재고 설정 (레시피 추가 시 호출 권장)
    void InitializeStock(const std::string& potionName) {
        stock[potionName] = MAX_STOCK;
    }

    bool DispensePotion(const std::string& potionName) {
        // 1. 재고 확인
        if (stock.find(potionName) == stock.end()) {
            std::cout << ">> '" << potionName << "'은(는) 등록되지 않은 물약입니다.\n";
            return false;
        }

        if (stock[potionName] >= 1) {
            stock[potionName]--;
            std::cout << ">> '" << potionName << "' 제공 완료. (남은 재고: " << stock[potionName] << ")\n";
            return true;
        }

        std::cout << ">> 재고 부족!\n";
        return false;
    }

    void ReturnPotion(const std::string& potionName) {
        if (stock.find(potionName) != stock.end() && stock[potionName] < MAX_STOCK) {
            stock[potionName]++;
            std::cout << ">> 반환 완료. (현재 재고: " << stock[potionName] << ")\n";
        }
        else {
            std::cout << ">> 반환 불가 (미등록 물약이거나 재고 풀)\n";
        }
    }
};

class AlchemyWorkshop {
private:
    std::vector<PotionRecipe> recipes;
    StockManager& stockManager; // 재고 관리자와 연결 (참조자 사용)

public:
    // 생성자에서 재고 관리자를 주입받음 (의존성 주입 기초)
    AlchemyWorkshop(StockManager& sm) : stockManager(sm) {}

    void addRecipe(const std::string& name, const std::vector<std::string>& ingredients) {
        recipes.push_back(PotionRecipe(name, ingredients));
        stockManager.InitializeStock(name); // 레시피 추가 시 재고도 초기화
        std::cout << ">> 레시피 '" << name << "' 추가 및 초기 재고 설정 완료.\n";
    }

    void displayAllRecipes() const {
        if (recipes.empty()) {
            std::cout << "등록된 레시피 없음.\n";
            return;
        }
        for (const auto& r : recipes) {
            std::cout << "- " << r.potionName << " (재료: ";
            for (size_t i = 0; i < r.ingredients.size(); ++i) {
                std::cout << r.ingredients[i] << (i < r.ingredients.size() - 1 ? ", " : "");
            }
            std::cout << ")\n";
        }
    }
};

int main() {
    StockManager myStock;
    AlchemyWorkshop myWorkshop(myStock); // 공방이 재고 관리자를 알게 함

    // 1. 레시피 추가 예시
    myWorkshop.addRecipe("치유 물약", { "약초", "물" });

    // 2. 재고 관리 테스트
    std::cout << "\n--- [ 재고 테스트 ] ---\n";
    myStock.DispensePotion("치유 물약");
    myStock.DispensePotion("마나 물약"); // 미등록 물약 테스트
    myStock.ReturnPotion("치유 물약");

    return 0;
}