#!/usr/bin/env python3
"""
从 SW Arch 组件列表生成 Combination matrix 骨架。

推荐 DFA 填写流程（ISO 26262 Part 9）：
  1. SW Arch        — 定义组件、ASIL、依赖关系（输入源）
  2. Combination matrix — 识别需要分析的组件对，分配 SW_DF_XXX ID
  3. Top Event      — 汇总每个 SW_DF_XXX 的 Top Event、CCF/CF 分类
  4. SW_Safety_Analysis — 对每个 Top Event 做 DF-I Category 耦合因子分析
  5. 安全机制        — 定义 Counter Measure 并回填
  6. Guide-Word     — 对接口信号做引导词分析（可选，针对通信路径）

用法:
  python tools/generate_dfa_matrix.py C255-10_SW_DFA报告.xlsx
"""

import sys
from pathlib import Path

import openpyxl
import pandas as pd


def read_sw_arch(xl: pd.ExcelFile) -> list[dict]:
    df = pd.read_excel(xl, "SW Arch", header=None)
    components = []
    for _, row in df.iterrows():
        cid = row[1]
        name = row[2]
        asil = row[4]
        if pd.notna(cid) and str(cid).startswith("SWC-"):
            components.append(
                {
                    "id": str(cid),
                    "name": str(name).strip(),
                    "asil": str(asil).strip() if pd.notna(asil) else "QM",
                }
            )
    return components


def needs_analysis(asil_a: str, asil_b: str) -> bool:
    """判断两个组件之间是否需要进行 DFA 分析。"""
    if asil_a == "QM" and asil_b == "QM":
        return False
    return True


def suggest_failure_type(name_a: str, name_b: str, layer_hint: dict) -> str:
    """根据组件层级关系推测 CCF 或 CF。"""
    layer_a = layer_hint.get(name_a, "ASW")
    layer_b = layer_hint.get(name_b, "ASW")
    # BSW 底层共享资源 -> 共因失效
    bsw_shared = {"SysClk", "NVM", "GPIO", "CAN", "ADC", "WDG", "PWM", "SPI"}
    if name_a in bsw_shared or name_b in bsw_shared:
        if layer_a == "BSW" or layer_b == "BSW":
            return "CCF"
    # ASW -> BSW/ASW 数据流 -> 级联失效
    if layer_a == "ASW" and layer_b in ("ASW", "BSW"):
        return "CF"
    return "CCF"


def generate_matrix_pairs(components: list[dict]) -> list[dict]:
    """生成需要分析的组件对（上三角）。"""
    layer_hint = {
        "SysClk": "BSW", "SPI": "BSW", "CAN": "BSW", "ADC": "BSW",
        "WDG": "BSW", "GPIO": "BSW", "PWM": "BSW", "NVM": "BSW",
        "CANBusOffDeal": "BSW", "CANReceive": "BSW", "CANTrans": "BSW",
        "UDS Server": "BSW", "EepromDriver": "BSW", "DEM": "BSW",
    }
    pairs = []
    for i, a in enumerate(components):
        for b in components[i + 1:]:
            if needs_analysis(a["asil"], b["asil"]):
                pairs.append(
                    {
                        "source": a["name"],
                        "target": b["name"],
                        "asil_a": a["asil"],
                        "asil_b": b["asil"],
                        "suggested_type": suggest_failure_type(
                            a["name"], b["name"], layer_hint
                        ),
                    }
                )
    return pairs


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)

    path = Path(sys.argv[1])
    xl = pd.ExcelFile(path)
    components = read_sw_arch(xl)
    pairs = generate_matrix_pairs(components)

    print(f"组件数: {len(components)}")
    print(f"需分析组件对（上三角）: {len(pairs)}")
    print()
    print("推荐填写顺序:")
    print("  SW Arch → Combination matrix → Top Event → SW_Safety_Analysis → 安全机制")
    print()
    print("待分配 SW_DF_XXX 的组件对（需人工确认依赖关系后填入 matrix）:")
    print(f"{'序号':<4} {'源组件':<20} {'目标组件':<20} {'ASIL':<15} {'建议类型'}")
    print("-" * 75)
    for idx, p in enumerate(pairs, 1):
        asil = f"{p['asil_a']}/{p['asil_b']}"
        print(f"{idx:<4} {p['source']:<20} {p['target']:<20} {asil:<15} {p['suggested_type']}")


if __name__ == "__main__":
    main()
