#pragma once

struct UniqueTag {};

template <typename Tag>
concept UniqueTagConcept = std::is_base_of_v<UniqueTag, Tag>;