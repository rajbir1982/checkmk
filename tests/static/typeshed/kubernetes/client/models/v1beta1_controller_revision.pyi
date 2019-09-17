# Stubs for kubernetes.client.models.v1beta1_controller_revision (Python 2)
#
# NOTE: This dynamically typed stub was automatically generated by stubgen.

from typing import Any, Optional

class V1beta1ControllerRevision:
    swagger_types: Any = ...
    attribute_map: Any = ...
    discriminator: Any = ...
    api_version: str = ...
    data: Any = ...
    kind: str = ...
    metadata: Any = ...
    revision: Any = ...
    def __init__(self, api_version: Optional[Any] = ..., data: Optional[Any] = ..., kind: Optional[Any] = ..., metadata: Optional[Any] = ..., revision: Optional[Any] = ...) -> None: ...
    @property
    def api_version(self) -> str: ...
    @api_version.setter
    def api_version(self, api_version: str) -> None: ...
    @property
    def data(self): ...
    @data.setter
    def data(self, data: Any) -> None: ...
    @property
    def kind(self) -> str: ...
    @kind.setter
    def kind(self, kind: str) -> None: ...
    @property
    def metadata(self): ...
    @metadata.setter
    def metadata(self, metadata: Any) -> None: ...
    @property
    def revision(self): ...
    @revision.setter
    def revision(self, revision: Any) -> None: ...
    def to_dict(self): ...
    def to_str(self): ...
    def __eq__(self, other: Any): ...
    def __ne__(self, other: Any): ...