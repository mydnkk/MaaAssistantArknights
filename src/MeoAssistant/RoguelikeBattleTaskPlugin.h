#pragma once

#include "AbstractTaskPlugin.h"
#include "AsstTypes.h"
#include "TilePack.h"
#include "BattleImageAnalyzer.h"

namespace asst
{
    enum class SkillUsage
    {
        Possibly = 0,   // 有就用，例如干员 棘刺 3 技能
        Once = 1,       // 只用一次，例如干员 银灰 2技能
        InTime = 2,     // 关键时刻使用，例如干员 假日威龙陈 3 技能
        OnceUsed
    };
    class RoguelikeBattleTaskPlugin : public AbstractTaskPlugin
    {
        using Loc = asst::TilePack::BuildableType;
        using Role = asst::BattleImageAnalyzer::Role;
    public:
        using SkillUsageMap = std::unordered_map<std::string, SkillUsage>;
    public:
        using AbstractTaskPlugin::AbstractTaskPlugin;
        virtual ~RoguelikeBattleTaskPlugin() = default;

        virtual bool verify(AsstMsg msg, const json::value& details) const override;

        void set_skill_usage(SkillUsageMap usage_map);
        void set_stage_name(std::string stage);

    protected:
        virtual bool _run() override;

        bool get_stage_info();
        bool auto_battle();
        bool speed_up();
        bool use_skill(const Rect& rect);
        void clear();

        // 通过资源文件离线计算可放置干员的位置，优先使用
        // 返回 可格子的位置
        Point get_placed(Loc buildable_type);

        // 计算摆放干员的朝向
        // 返回滑动的方向
        Point calc_direction(Point loc, Role role);

        bool m_opers_used = false;
        int m_pre_hp = 0;

        std::unordered_map<Point, TilePack::TileInfo> m_side_tile_info;
        std::unordered_map<Point, TilePack::TileInfo> m_normal_tile_info;
        std::vector<Point> m_homes;
        size_t m_cur_home_index = 0;
        std::unordered_map<Point, std::string> m_used_tiles;

        std::string m_stage_name;
        SkillUsageMap m_skill_usage;
    };
}
